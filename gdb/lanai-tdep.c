/* Common target dependent code for GDB on LANAI systems.

   Copyright (C) 1988, 1989, 1991, 1992, 1993, 1995, 1996, 1998, 1999, 2000,
   2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010
   Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <ctype.h>		/* XXX for isupper () */

#include "defs.h"
#include "frame.h"
#include "inferior.h"
#include "gdbcmd.h"
#include "gdbcore.h"
#include "gdb_string.h"
#include "dis-asm.h"		/* For register styles. */
#include "regcache.h"
#include "doublest.h"
#include "value.h"
#include "arch-utils.h"
#include "osabi.h"
#include "frame-unwind.h"
#include "frame-base.h"
#include "trad-frame.h"
#include "objfiles.h"
#include "dwarf2-frame.h"
#include "gdbtypes.h"
#include "prologue-value.h"
#include "target-descriptions.h"
#include "user-regs.h"

#include "bfd-in2.h"

#include "lanai-tdep.h"

static const char *reg_name[] = {
  "r0", "r1", "pc", "ps", "sp", "fp", "r6", "ra", "rv", "r9",
  "r10","r11","r12","r13","r14","rca","r16","r17","r18","r19",
  "r20","r21","r22","r23","r24","r25","r26","r27","r28","r29",
  "r30","r31"
};

static const char *
lanai_register_name (struct gdbarch *a, int regno)
{
  return (unsigned int)regno<32 ? reg_name[regno] : 0;
}


static CORE_ADDR
lanai_unwind_sp (struct gdbarch *gdbarch, struct frame_info *next_frame)
{
  return frame_unwind_register_unsigned (next_frame, LANAI_SP_REGNUM);
}

static CORE_ADDR
lanai_unwind_pc (struct gdbarch *gdbarch, struct frame_info *this_frame)
{
  return frame_unwind_register_unsigned (this_frame, LANAI_PC_REGNUM);
}


static ULONGEST
lanai_read_integer (struct gdbarch*arch, CORE_ADDR addr)
{
  char buf[sizeof (ULONGEST)];

  target_read_memory (addr, buf, 4);
  return extract_unsigned_integer (buf, 4, gdbarch_byte_order (arch));
}

static CORE_ADDR
lanai_skip_prologue (struct gdbarch *a, CORE_ADDR pc)
{
  int insn;
  
  /* Recognize the Lanai prologue:
     st %fp,-4[*%sp]		!push old FP
     add %sp,8,%fp		!generate new FP
     sub %sp,<#>,%sp		!allocate stack space
  */

  insn = lanai_read_integer (a, pc);
  
  if (insn == 0x9293FFFC)
  {
    insn = lanai_read_integer (a, pc + 4);
    if (insn == 0x02900008)
    {
      pc += 8;
      insn = lanai_read_integer (a, pc);
      if ((insn >> 16) == 0x2210)
	return pc + 4;
    }
  }
  return pc;
}


static unsigned char lanai_breakpoint [] = {0xff, 0xff, 0xff, 0xff};

static const unsigned char *
lanai_breakpoint_from_pc (struct gdbarch *gdbarch,
			  CORE_ADDR *pcptr,
			  int *lenptr)
{
  *lenptr = sizeof (lanai_breakpoint);
  return lanai_breakpoint;
}


/* Return the GDB type object for the "standard" data type of data in
   register N.  */

static struct type *
lanai_register_type (struct gdbarch *gdbarch, int regnum)
{
  return builtin_type (gdbarch)->builtin_uint32;
}

struct lanai_frame_cache
{
  CORE_ADDR base;
  CORE_ADDR prev_pc;
  CORE_ADDR prev_fp;
  
  struct trad_frame_saved_reg *saved_regs;
};


static struct lanai_frame_cache *
lanai_frame_cache (struct frame_info *this_frame, void **this_cache)
{
  struct lanai_frame_cache *cache;

  if ((*this_cache))
    return (*this_cache);

  cache = FRAME_OBSTACK_ZALLOC (struct lanai_frame_cache);
  (*this_cache) = cache;

  cache->saved_regs = trad_frame_alloc_saved_regs (this_frame);
  cache->base = get_frame_register_unsigned (this_frame, LANAI_FP_REGNUM);

  /*only try to read the previous frame regs if there is one*/
  if (cache->base)
  {
    cache->prev_pc = get_frame_memory_unsigned (this_frame, cache->base - 4, 4);
    cache->prev_fp = get_frame_memory_unsigned (this_frame, cache->base - 8, 4);
  }
  
  return cache;
}


static struct value *lanai_frame_prev_register (struct frame_info *this_frame,
						void **this_cache, int regnum)
{
  static struct lanai_frame_cache*cache;

  cache = lanai_frame_cache (this_frame, this_cache);
  if (regnum == LANAI_FP_REGNUM)
    return frame_unwind_got_constant (this_frame, regnum, cache->prev_fp);
  if (regnum == LANAI_PC_REGNUM)
    return frame_unwind_got_constant (this_frame, regnum, cache->prev_pc);
  if (regnum == LANAI_SP_REGNUM)
    return frame_unwind_got_constant (this_frame, regnum, cache->base);
  
  return trad_frame_get_prev_register (this_frame, cache->saved_regs, regnum);
}



static void
lanai_frame_this_id (struct frame_info *this_frame, void **this_cache,
		    struct frame_id *this_id)
{
  struct lanai_frame_cache *cache = lanai_frame_cache (this_frame, this_cache);

  /* This marks the outermost frame.  */
  if (cache->base == 0)
    return;

  (*this_id) = frame_id_build (cache->base, get_frame_func (this_frame));
}

static CORE_ADDR
lanai_frame_base_address (struct frame_info *this_frame, void **this_cache)
{
  struct lanai_frame_cache *cache =
    lanai_frame_cache (this_frame, this_cache);

  return cache->base;
}


static const struct frame_unwind lanai_frame_unwind =
{
  NORMAL_FRAME,
  lanai_frame_this_id,
  lanai_frame_prev_register,
  NULL,
  default_frame_sniffer
};


static const struct frame_base lanai_frame_base = 
{
  &lanai_frame_unwind,
  lanai_frame_base_address,
  lanai_frame_base_address,
  lanai_frame_base_address
};


/*these are provided by the simulator*/
extern int sim_insert_breakpoint (unsigned address);
extern int sim_remove_breakpoint (unsigned address);

int lanai_memory_insert_breakpoint (struct gdbarch *gdbarch, struct bp_target_info *bp_tgt)
{
 return !sim_insert_breakpoint (bp_tgt->placed_address);
}
int lanai_memory_remove_breakpoint (struct gdbarch *gdbarch, struct bp_target_info *bp_tgt)
{
  return !sim_remove_breakpoint (bp_tgt->placed_address);
}


/* Initialize the current architecture based on INFO.  If possible,
   re-use an architecture from ARCHES, which is a list of
   architectures already created during this debugging session.

   Called e.g. at program startup, when reading a core file, and when
   reading a binary file.  */



static struct gdbarch *
lanai_gdbarch_init (struct gdbarch_info info, struct gdbarch_list *arches)
{
  struct gdbarch_tdep *tdep;
  struct gdbarch *gdbarch;
  
  tdep = xcalloc (1, sizeof (struct gdbarch_tdep));
  gdbarch = gdbarch_alloc (&info, tdep);
  set_gdbarch_num_regs (gdbarch, 32);
  set_gdbarch_register_type (gdbarch, lanai_register_type);
  set_gdbarch_register_name (gdbarch, lanai_register_name);
  set_gdbarch_skip_prologue (gdbarch, lanai_skip_prologue);
  set_gdbarch_inner_than (gdbarch, core_addr_lessthan);
  set_gdbarch_breakpoint_from_pc (gdbarch, lanai_breakpoint_from_pc);
  set_gdbarch_print_insn (gdbarch, print_insn_lanai);
  set_gdbarch_unwind_pc (gdbarch, lanai_unwind_pc);
  set_gdbarch_unwind_sp (gdbarch, lanai_unwind_sp);
  set_gdbarch_pc_regnum (gdbarch, LANAI_PC_REGNUM);
  set_gdbarch_sp_regnum (gdbarch, LANAI_SP_REGNUM);

  set_gdbarch_memory_remove_breakpoint (gdbarch, lanai_memory_remove_breakpoint);
  set_gdbarch_memory_insert_breakpoint (gdbarch, lanai_memory_insert_breakpoint);
  
  frame_base_set_default (gdbarch, &lanai_frame_base);
  frame_unwind_append_unwinder (gdbarch, &lanai_frame_unwind);
  
  return gdbarch;
}

static void
lanai_dump_tdep (struct gdbarch *gdbarch, struct ui_file *file)
{
  struct gdbarch_tdep *tdep = gdbarch_tdep (gdbarch);

  if (tdep == NULL)
    return;

  fprintf_unfiltered (file, _("lanai_dump_tdep: Lowest pc = 0x%lx"),
		      (unsigned long) tdep->lowest_pc);
}

extern initialize_file_ftype _initialize_lanai_tdep; /* -Wmissing-prototypes */

void
_initialize_lanai_tdep (void)
{
  gdbarch_register (bfd_arch_lanai, lanai_gdbarch_init, lanai_dump_tdep);
}
