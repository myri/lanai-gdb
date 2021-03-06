/* Register support routines for the remote server for GDB.
   Copyright (C) 2001, 2002, 2007, 2008, 2009, 2010
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

#ifndef REGCACHE_H
#define REGCACHE_H

struct inferior_list_entry;
struct thread_info;

/* The data for the register cache.  Note that we have one per
   inferior; this is primarily for simplicity, as the performance
   benefit is minimal.  */

struct regcache
{
  int registers_valid;
  unsigned char *registers;
};

/* Create a new register cache for INFERIOR.  */

struct regcache *new_register_cache (void);

struct regcache *get_thread_regcache (struct thread_info *thread, int fetch);

/* Release all memory associated with the register cache for INFERIOR.  */

void free_register_cache (struct regcache *regcache);

/* Invalidate cached registers for one or all threads.  */

void regcache_invalidate_one (struct inferior_list_entry *);
void regcache_invalidate (void);

/* Convert all registers to a string in the currently specified remote
   format.  */

void registers_to_string (struct regcache *regcache, char *buf);

/* Convert a string to register values and fill our register cache.  */

void registers_from_string (struct regcache *regcache, char *buf);

/* Return a pointer to the description of register ``n''.  */

struct reg *find_register_by_number (int n);

int register_size (int n);

int find_regno (const char *name);

/* The following two variables are set by auto-generated
   code in the init_registers_... routines.  */
extern const char **gdbserver_expedite_regs;
extern const char *gdbserver_xmltarget;

void supply_register (struct regcache *regcache, int n, const void *buf);

void supply_register_by_name (struct regcache *regcache,
			      const char *name, const void *buf);

void collect_register (struct regcache *regcache, int n, void *buf);

void collect_register_as_string (struct regcache *regcache, int n, char *buf);

void collect_register_by_name (struct regcache *regcache, const char *name, void *buf);

#endif /* REGCACHE_H */
