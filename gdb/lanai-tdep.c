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
  "r0", "r1", "pc", "psw", "r4", "r5", "r6", "r7", "rv", "r9",
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
lanai_unwind_pc (struct gdbarch *gdbarch, struct frame_info *this_frame)
{
  return frame_unwind_register_unsigned (this_frame, LANAI_PC_REGNUM);
}

static CORE_ADDR
lanai_skip_prologue (struct gdbarch *a, CORE_ADDR pc)
{
  return pc+8;
}

static const unsigned char *
lanai_breakpoint_from_pc (struct gdbarch *gdbarch,
			  CORE_ADDR *pcptr,
			  int *lenptr)
{
  *lenptr = 0;
  return 0;			/* ??? */
}


/* Return the GDB type object for the "standard" data type of data in
   register N.  */

static struct type *
lanai_register_type (struct gdbarch *gdbarch, int regnum)
{ 

    return builtin_type (gdbarch)->builtin_uint32;
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
