/* Common target dependent code for GDB on ARM systems.
   Copyright (C) 2002, 2003, 2007, 2008, 2009, 2010
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

#ifndef LANAI_TDEP_H
#define LANAI_TDEP_H

/* Target-dependent structure in gdbarch.  */
struct gdbarch_tdep
{
  CORE_ADDR lowest_pc;	      /* Lowest address at which instructions 
				 will appear.  */
};


/* Register numbers of various important registers.  */


/* Register numbers of various important registers.
   Note that some of these values are "real" register numbers,
   and correspond to the general registers of the machine,
   and some are "phony" register numbers which are too large
   to be actual register numbers as far as the user is concerned
   but do serve to get the desired values when passed to read_register.  */

enum lanai_regnum
{
  LANAI_R0_REGNUM = 0,  /* general register 0 (always == 0)        */
  LANAI_PC_REGNUM = 2,	/* program counter                         */
  LANAI_PS_REGNUM = 3,	/* processor status                        */
  LANAI_SP_REGNUM = 4,	/* address of top of stack/bottom of frame */
  LANAI_FP_REGNUM = 5,	/* address of executing stack frame        */
  LANAI_RCA_REGNUM = 15,	/* return continuation address             */
  LANAI_LAST_REGNUM = 31  /* last general register                   */
};

  

#endif /* lanai-tdep.h */
