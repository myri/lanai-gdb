/*									tab:8
 *
 * tm-lanai3.h - macro definitions for GDB for a Lanai processor
 *
 * "Copyright (c) 1997 by Steve Lumetta and The Regents of the University 
 * of California.  All rights reserved."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Steve Lumetta
 * Version:	    1
 * Creation Date:   Thu Jan 30 13:13:17 1997
 * Filename:	    tm-lanai3.h
 * History:
 *	SL	1	Thu Jan 30 13:13:17 1997
 *		Header added.  This file was molded from the GDB
 *			file config/sparc/tm-sun4sol2.h (hence the
 *			GDB copyright below).
 */

/* Macro definitions for GDB for a Lanai processor
   Copyright 1989, 1992, 1993, 1994, 1995 Free Software Foundation, Inc.

This file is part of GDB.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#define TARGET_BYTE_ORDER BIG_ENDIAN

/* If an argument is declared "register", Sun cc will keep it in a register,
   never saving it onto the stack.  So we better not believe the "p" symbol
   descriptor stab.  */

#define USE_REGISTER_NOT_ARG

/* When passing a structure to a function, Sun cc passes the address
   not the structure itself.  It (under SunOS4) creates two symbols,
   which we need to combine to a LOC_REGPARM.  Gcc version two (as of
   1.92) behaves like sun cc.  REG_STRUCT_HAS_ADDR is smart enough to
   distinguish between Sun cc, gcc version 1 and gcc version 2.  */

#define REG_STRUCT_HAS_ADDR(gcc_p,type) (gcc_p != 1)

/* Sun /bin/cc gets this right as of SunOS 4.1.x.  We need to define
   BELIEVE_PCC_PROMOTION to get this right now that the code which
   detects gcc2_compiled. is broken.  This loses for SunOS 4.0.x and
   earlier.  */

#define BELIEVE_PCC_PROMOTION 1

/* Variables in the debug stabs occur before the N_LBRAC. */
#define VARIABLES_INSIDE_BLOCK(desc, gcc_p) 0

/* Offset from address of function to start of its code.
   Zero on most machines.  */

#define FUNCTION_START_OFFSET 0

/* Advance PC across any function entry prologue instructions
   to reach some "real" code.  Its result is equal to its input 
   PC if the function is frameless, unequal otherwise.  */

#define SKIP_PROLOGUE(pc) { pc = examine_prologue (pc); }
extern CORE_ADDR examine_prologue PARAMS ((CORE_ADDR));

/* Immediately after a function call, return the saved pc.
   Can't go through the frames for this because on some machines
   the new frame is not set up until the new function executes
   some instructions.  */

#define SAVED_PC_AFTER_CALL(frame) read_register (RCA_REGNUM)

/* Stack grows downward.  */

#define INNER_THAN <

/* Stack has strict alignment.  */

#define STACK_ALIGN(ADDR) (((ADDR)+7)&-8)

/* 
   The breakpoint sequence defined here is not actually used in lanai.c.
   This serves only to make various other modules aware of the size of
   a Lanai breakpoint.

   A general breakpoint sequence for Lanai (one that saves the PC and RCA
   registers) takes 5 full instructions and tends to cause problems with
   overlap and single-step continue.  Instead, we jump to a distinct
   routine for each breakpoint using a 2 instruction sequence.

   Sequence of bytes for Lanai breakpoint sequence:

   mov 0,%pc   ! jump to the breakpoint handler for this breakpoint
	       ! (fixed in lanai_open in lanai.c)
   nop         ! delay slot for jump--make sure nothing weird happens

   This is the general sequence...

	nop                  ! make sure any loads to rca are complete
	st %rca,[--%sp]      ! push the rca value onto the stack
        sub %pc,12,%rca      ! find the return address
        mov _lanai_break,%pc ! call the breakpoint routine
	                     ! (fixed in lanai_open in lanai.c)
        st %rca,[--%sp]      ! push the return address
*/
/*finucane changed this

#define BREAKPOINT {0xE0, 0x00, 0x00, 0x00, \
		    0x00, 0x00, 0x00, 0x01}
*/

#define BREAKPOINT {0xff, 0xff, 0xff, 0xff}

/* Treat the second instruction in a breakpoint as nullified.  (Always 
   step past it rather than stopping on it?  The effect we care about is 
   that breakpoints aren't inserted until you're past the entire 
   breakpoint sequence.) */

/*finucane changed this
#define INSTRUCTION_NULLIFIED (breakpoint_here_p (stop_pc - 4))
*/
#define INSTRUCTION_NULLIFIED 0

/* Amount PC must be decremented by after a breakpoint.
   This is often the number of bytes in BREAKPOINT
   but not always.  */

#define DECR_PC_AFTER_BREAK 0

/* Say how long (ordinary) registers are.  This is a piece of bogosity
   used in push_word and a few other places; REGISTER_RAW_SIZE is the
   real way to know how big a register is.  */

#define REGISTER_SIZE 4

/* Number of machine registers */

#define NUM_REGS 32

/* Initializer for an array of names of registers.
   There should be NUM_REGS strings in this initializer.  */

#define REGISTER_NAMES                                                  \
{ "r0", "r1", "pc", "ps", "sp", "fp", "r6", "ra", "rv", "r9",           \
  "r10", "r11", "r12", "r13", "r14", "rca", "r16", "r17", "r18", "r19", \
  "r20", "r21", "r22", "r23", "r24", "r25", "r26", "r27", "aps", "apc", \
  "imr", "isr"}

/* Register numbers of various important registers.
   Note that some of these values are "real" register numbers,
   and correspond to the general registers of the machine,
   and some are "phony" register numbers which are too large
   to be actual register numbers as far as the user is concerned
   but do serve to get the desired values when passed to read_register.  */

#define R0_REGNUM    0  /* general register 0 (always == 0)        */
#define	PC_REGNUM    2	/* program counter                         */
#define	PS_REGNUM    3	/* processor status                        */
#define	SP_REGNUM    4	/* address of top of stack/bottom of frame */
#define	FP_REGNUM    5	/* address of executing stack frame        */
#define RCA_REGNUM  15	/* return continuation address             */
#define IMR_REGNUM  30  /* interrupt mask register                 */
#define LAST_REGNUM 31  /* last general register                   */

/* Total amount of space needed to store our copies of the machine's
   register state, the array `registers'.  On the sparc, `registers'
   contains the ins and locals, even though they are saved on the
   stack rather than with the other registers, and this causes hair
   and confusion in places like pop_frame.  It might be
   better to remove the ins and locals from `registers', make sure
   that get_saved_register can get them from the stack (even in the
   innermost frame), and make this the way to access them.  For the
   frame pointer we would do that via TARGET_READ_FP.  On the other hand,
   that is likely to be confusing or worse for flat frames.  */

#define REGISTER_BYTES (32*4)

/* Index within `registers' of the first byte of the space for
   register N.  */
#define REGISTER_BYTE(N)  ((N)*4)

#define GET_SAVED_REGISTER 1

/* Number of bytes of storage in the actual machine representation
   for register N.  */

#define REGISTER_RAW_SIZE(N) 4

/* Number of bytes of storage in the program's representation
   for register N.  */

#define REGISTER_VIRTUAL_SIZE(N) 4

/* Largest value REGISTER_RAW_SIZE can have.  */

#define MAX_REGISTER_RAW_SIZE 4

/* Largest value REGISTER_VIRTUAL_SIZE can have.  */

#define MAX_REGISTER_VIRTUAL_SIZE 4

/* Return the GDB type object for the "standard" data type
   of data in register N.  */

#define REGISTER_VIRTUAL_TYPE(N) builtin_type_int

/* We disallow modification of R0 and R1 (nop's), the SP (tricky and
   not worth the extra code size), the FP (for consistency with the
   SP), and all registers for the alternate context (since the cached
   register values for a running context are most likely invalid).
*/

#define CANNOT_STORE_REGISTER(regno)                \
((regno) != PC_REGNUM && (regno) != PS_REGNUM       \
 && (regno) != IMR_REGNUM                           \
 && ((regno) <= FP_REGNUM || (regno) > RCA_REGNUM))

/* Store the address of the place in which to copy the structure the
   subroutine will return.  This is called from call_function. */

#define STORE_STRUCT_RETURN(ADDR, SP)

/* Extract from an array REGBUF containing the (raw) register state
   a function return value of type TYPE, and copy that, in virtual format,
   into VALBUF.  */

#define EXTRACT_RETURN_VALUE(TYPE,REGBUF,VALBUF)

/* Write into appropriate registers a function return value
   of type TYPE, given in virtual format.  */
#define STORE_RETURN_VALUE(TYPE,VALBUF)


/* Describe the pointer in each stack frame to the previous stack frame
   (its caller).  */

/* FRAME_CHAIN takes a frame's nominal address
   and produces the frame's chain-pointer. */

#define EXTRA_FRAME_INFO  \
  CORE_ADDR pc_addr;

#define INIT_EXTRA_FRAME_INFO(fromleaf, fci) \
  lanai_init_extra_frame_info (fci)
#ifdef __STDC__
struct frame_info;
#endif
extern void lanai_init_extra_frame_info PARAMS ((struct frame_info* fci));

#define FRAME_CHAIN(thisframe) (thisframe->pc_addr)

#define	INIT_FRAME_PC(fromleaf, prev) /* nothing */

/* Define other aspects of the stack frame.  */

/* Where is the PC for a specific frame */

#define FRAME_SAVED_PC(FRAME) read_memory_integer (FRAME->frame - 4, 4)

/* If the argument is on the stack, it will be here.  */
#define FRAME_ARGS_ADDRESS(fi) ((fi)->frame)

#define FRAME_STRUCT_ARGS_ADDRESS(fi) ((fi)->frame)

#define FRAME_LOCALS_ADDRESS(fi) ((fi)->frame)

/* Set VAL to the number of args passed to frame described by FI.
   Can set VAL to -1, meaning no way to tell.  */

/* We can't tell how many args there are
   now that the C compiler delays popping them. */
#define FRAME_NUM_ARGS(val,fi) (val = -1)

/* Return number of bytes at start of arglist that are not really args. */

#define FRAME_ARGS_SKIP 0

#define POP_FRAME \
    error ("Popping stack frames is not supported for the Lanai.")

/* We must single step by hand. */

#define NO_SINGLE_STEP 1
extern void single_step PARAMS ((int));

/* We need more arguments in a frame specification for the
   "frame" or "info frame" command.  */

#define SETUP_ARBITRARY_FRAME(argc, argv) setup_arbitrary_frame (argc, argv)
extern struct frame_info *setup_arbitrary_frame PARAMS ((int, CORE_ADDR *));

/* The Lanai does not have signals. */
#define IN_SIGTRAMP(pc, name) 0

/* Define a new default prompt. */
#define DEFAULT_PROMPT "(lcpgdb) "

/* Disable the run command. */
#define DISABLE_RUN_COMMAND error ("Use attach [system|user].")

/* Allow re-attach to work silently. */
#define SILENT_REATTACH


/************************************************************************
 * Revision History
 *
 * $Log: tm-lanai3.h,v $
 * Revision 1.1.1.1  2002-11-06 00:35:56  finucane
 * simulator
 *
 * Revision 1.1  1997/01/31 06:25:15  stevel
 * First version of lcpgdb
 *
 *
 ************************************************************************/
