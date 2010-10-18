#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <bfd.h>
#include <signal.h>

extern "C"
{
  
#include "gdb/callback.h"
#include "gdb/remote-sim.h"
#include "ansidecl.h"
#include "sim-utils.h"
#include "run-sim.h"
#include "gdb/signals.h"
#include "../common/sim-utils.h"

#define CORE_ADDR unsigned
#include "../gdb/lanai-tdep.h"

  void set_prompt (char *s);
  int sim_insert_breakpoint (unsigned address);
  int sim_remove_breakpoint (unsigned address);
}

#include "gs_client.h"
#include "insist.h"

static SIM_OPEN_KIND sim_kind;
static char *myname;
host_callback *sim_callback;

gs_client_t client;


SIM_DESC sim_open PARAMS ((SIM_OPEN_KIND kind, struct host_callback_struct *callback, struct bfd *abfd, char **argv))
{
  try
  {
    sim_callback = callback;
    myname = argv [0];  

    set_prompt ((char*)"(lanai) ");

    return (SIM_DESC) 1;
  }
  catch (exception_t&e)
  {
    e.print ();
    return 0;
  }

}

void sim_close PARAMS ((SIM_DESC sd, int quitting))
{
  try
  {  
    //sim_callback->printf_filtered (sim_callback, "sim_close\n");
  }
  catch (exception_t&e)
  {
    e.print ();
  }
}

static bfd*prog_bfd = 0;

SIM_RC sim_load PARAMS ((SIM_DESC sd, char *prog, struct bfd *abfd, int from_tty))
{
  try
  {
    //sim_callback->printf_filtered (sim_callback, "sim_load\n");

    /*keep track of the bfd for segfault stuff*/

    prog_bfd = sim_load_file (sd, myname, sim_callback, prog, abfd, sim_kind == SIM_OPEN_DEBUG, 0, sim_write);
    if (prog_bfd == NULL)
      return SIM_RC_FAIL;

    return SIM_RC_OK;
  }
  catch (exception_t&e)
  {
    e.print ();
    return SIM_RC_FAIL;
  }
}

SIM_RC sim_create_inferior PARAMS ((SIM_DESC sd, struct bfd *abfd, char **argv, char **env))
{
  try
  {
    //sim_callback->printf_filtered (sim_callback, "sim_create_inferior\n");
    return SIM_RC_OK;
  }
  catch (exception_t&e)
  {
    e.print ();
    return SIM_RC_FAIL;
  }

}

int sim_read PARAMS ((SIM_DESC sd, SIM_ADDR mem, unsigned char *buf, int length))
{
  try
  {
    //sim_callback->printf_filtered (sim_callback, "sim_read address is 0x%x, length is %d\n", mem, length);
    client.read_memory (mem, length, (char*)buf);
    return 1;
  }
  catch (exception_t&e)
  {
    e.print ();
    return 0;
  }

}

int sim_write PARAMS ((SIM_DESC sd, SIM_ADDR mem, unsigned char *buf, int length))
{
  try
  { 
    //sim_callback->printf_filtered (sim_callback, "sim_write\n");    
    client.write_memory (mem, length, (char*)buf);
    return 1;
  }
  catch (exception_t&e)
  {
    e.print ();
    return 0;
  }
}

int sim_fetch_register PARAMS ((SIM_DESC sd, int regno, unsigned char *buf, int length))
{
  try
  {
    //sim_callback->printf_filtered (sim_callback, "sim_fetch_register\n");
    unsigned n = client.fetch_register (regno);
    memcpy (buf, (char*)&n, sizeof (n));

    return 4;
  }
  catch (exception_t&e)
  {
    e.print ();
    return 0;
  }
}

int sim_store_register PARAMS ((SIM_DESC sd, int regno, unsigned char *buf, int length))
{
  try
  {
    //sim_callback->printf_filtered (sim_callback, "sim_store_register\n");   
    insist (0);
    unsigned n;
    memcpy ((void*) &n, buf, sizeof (n));
    client.store_register (regno, n);

    return 4;
  }
  catch (exception_t&e)
  {
    e.print ();
    return 0;
  }

}


typedef void (*sighandler_t)(int);
static sighandler_t old_signal_handler = 0;
static int reason;

/*this is redundant since we're able to detect a signal in the select call in client.wait ().
  but i'm leaving it in*/

static void signal_handler (int)
{
  //printf ("signal_handler\n");
  reason = SIGINT;
}


static unsigned read_memory_int (unsigned addr)
{
  unsigned n;
  client.read_memory (addr, sizeof (n), (char*)&n);
  return htonl (n);
}

/*if pc is at the start of a function prologue, return the new pc that
  is beyond the prologue. otherwise return an unchanged pc, which
  means we aren't in a function prologue.  we have to do this every
  time we single step so we end up skipping through function prologues
  because if we let the user stop inside a function prologue, the
  stack frame isn't set up yet and backtraces will look like garbage*/

static unsigned skip_prologue (unsigned pc)
{
  int insn;
  
  /* Recognize the Lanai prologue:
     st %fp,-4[*%sp]		!push old FP
     add %sp,8,%fp		!generate new FP
     sub %sp,<#>,%sp		!allocate stack space
  */

  insn = read_memory_int (pc);

  if (insn == 0x9293FFFC)
  {
    insn = read_memory_int (pc + 4);
    if (insn == 0x02900008)
    {
      pc += 8;
      insn = read_memory_int (pc);
      if ((insn >> 16) == 0x2210)
	return pc + 4;
    }
  }
  return pc;
}


void sim_resume PARAMS ((SIM_DESC sd, int step, int siggnal))
{
  if (!prog_bfd)
  {
    sim_callback->printf_filtered (sim_callback, "Use \"load\" to load a program first.\n");
    return;
  }

  try
  {
   
    //sim_callback->printf_filtered (sim_callback, "sim_resume, step is %d, siggnal is %d\n", step, siggnal);

    //old_signal_handler = signal (SIGINT, signal_handler);
    reason = 0;  

  
    /*remember the pc before we go, so we know if we have stepped past when we are stepping*/

    unsigned pc = client.fetch_register (LANAI_PC_REGNUM);
    pc = htonl (pc);
    
    /*now it is safe to let the lanai go again. the next message we get back from the lanai is going to be a break.*/
    client.resume (step);    

    while (!reason)
    {
      try
      {
	/*wait for the simulator on the other end of the connection to interrupt us (or for a C-c from the user, we don't care)*/
	/*there is a race condition in all of this. if the lanai hits a breakpoint before we send client._break () then
	  we'll get an unexpected BREAK message and assert. if this happens the answer is to ignore that BREAK.*/

	int r = client.wait ();

	if (r == 0)
	{
	  if (reason == 0) continue;
	  if (reason == SIGINT) r = -1;
	}
	else if (r > 0)
	  reason = (r == gs_client_t::BREAK ? SIGINT : SIGSEGV);

	//sim_callback->printf_filtered (sim_callback, "reason is %d\n",  reason);
	
	insist (r > 0 || reason == SIGINT);
	unsigned new_pc = htonl (client.fetch_register (LANAI_PC_REGNUM));
	
	/*if we were stepping, but we didn't change pc, keep going. we step at the cycle level*/
	if ( r > 0 && step && pc == new_pc)
	{
	  //sim_callback->printf_filtered (sim_callback, "didn't change pc, resuming\n");
	  reason = 0;
	  client.resume (step);
	  continue;
	}

	if (r < 0)
	{
	  /*we are going to trust that the new signal handling works, and any signal
	    that happens is going to trigger sim_stop. that's why we have commented out
	    the old_signal_handler stuff too.*/
	  insist (reason == SIGINT);
	  
	  //client._break ();
	  reason = SIGINT;
	  break;
	}
	
	reason = (r == gs_client_t::BREAK ? SIGTRAP : SIGSEGV);

	
	/*if we stopped in the start of a function prologue, silently skip past it*/
	unsigned skipped_pc = skip_prologue (new_pc);
	int skipped = 0;
	while (skipped_pc != new_pc)
	{
	  skipped = 1;
	  //sim_callback->printf_filtered (sim_callback, "skipped_pc is 0x%x, new_pc is 0x%x\n", skipped_pc, new_pc);
	  client.resume (1);
	  client.wait ();
	  new_pc = htonl (client.fetch_register (LANAI_PC_REGNUM));
	}

	if (skipped)
	{
	  /*this is the biggest kludge ever. let the cpu do 1 more cycle.
	    this is to let the cpu finish setting up the stack frame, seems to take 1 more cycle*/
	  
	  //	  client.resume (1);
	  //	  client.wait ();
	
	  //	  insist (htonl (client.fetch_register (LANAI_PC_REGNUM) == new_pc));
	}
	
	break;
      }
      catch (exception_t&e)
      {
	e.print ();
	reason = e.reason;
	break;
      }
      break;
    }

    //signal (SIGINT, old_signal_handler);
  }
  catch (exception_t&e)
  {
    e.print ();
    return;
  }   
}

int sim_stop PARAMS ((SIM_DESC sd))
{
  try
  {
    //sim_callback->printf_filtered (sim_callback, "sim_stop\n");
    reason = SIGINT;
    client._break ();
    return 1;
  }
  catch (exception_t&e)
  {
    e.print ();
    return 0;
  }

}

void sim_stop_reason PARAMS ((SIM_DESC sd, enum sim_stop *r, int *sigrc))
{
  //sim_callback->printf_filtered (sim_callback, "sim_stop_reason\n");
  *r = sim_stopped;
  *sigrc = reason == SIGSEGV ? TARGET_SIGNAL_BUS : TARGET_SIGNAL_TRAP;
}

/*bunch of hooks we don't care about*/
int sim_target_parse_command_line PARAMS ((int x, char **y))
{
  return x;
}
void sim_target_display_usage PARAMS ((int help))
{
}
void sim_set_callbacks PARAMS ((struct host_callback_struct *p))
{
  sim_callback = p;
}
void sim_size PARAMS ((int i))
{
}
int sim_trace PARAMS ((SIM_DESC sd))
{
  return 1;
}
void sim_info PARAMS ((SIM_DESC sd ATTRIBUTE_UNUSED, int verbose ATTRIBUTE_UNUSED))
{
}



typedef void (*sim_fun_t)(char*s);
typedef struct sim_cmd_t 
{
  const char*name;
  const char*usage;
  const char*full;
  const sim_fun_t fun;
}sim_cmd_t;

static int sim_cmd_lookup (char*s, int reverse=0);
static void sim_cmd_help (char*s);
static void sim_cmd_connect (char*s);
static void sim_cmd_watch_pc (char*s);
static void sim_cmd_detach (char*s);
static void sim_cmd_cycle (char*s);


static sim_cmd_t sim_cmd_table [] =
  {
    {"help", "print list of simulator commands.", "", sim_cmd_help},
    {"connect", "connect to verilog.", "type \"sim connect <hostname> <port> [<cpu> [num_cpus]]\"", sim_cmd_connect},
    {"watch_pc", "segfault if pc is ever outside of code sections", "type \"sim watch_pc\"", sim_cmd_watch_pc},
    {"detach", "detach from verilog simulation", "type \"sim detach\"", sim_cmd_detach},
    {"cycle", "cycle cpu 1 clock", "type \"sim cycle\"", sim_cmd_cycle}
  };
#define NUM_CMDS (int) (sizeof (sim_cmd_table) / sizeof (sim_cmd_table [0]))

void sim_do_command PARAMS ((SIM_DESC sd, char*s))
{
  while (s && *s && isspace (*s)) s++;

  int c = sim_cmd_lookup (s);
  if (c < 0)
    sim_callback->printf_filtered (sim_callback, "unknown command \"%s\"\n", s ? s : "none");
  else if (c >= NUM_CMDS)
    sim_callback->printf_filtered (sim_callback, "ambiguous command \"%s\"\n", s);
  else
  {
    while (*s && !isspace (*s)) s++;

    try
    {
      (*sim_cmd_table [c].fun) (s);
    }
    catch (exception_t&e)
    {
      e.print ();
    }
  }
}

int sim_cmd_lookup (char*s, int reverse)
{
  if (!s) return -1;
  
  int c = -1;

  while (*s && isspace (*s)) s++;

  for (int i = 0; i < NUM_CMDS; i++)
  {
    if (reverse ? strstr (sim_cmd_table [i].name, s) == sim_cmd_table [i].name : strstr (s, sim_cmd_table [i].name) == s)
    {
      if (c >= 0) return NUM_CMDS;
      c = i;
    }
  }
  return c;
}

void sim_cmd_help (char*s)
{
  insist (s);
  while (*s && isspace (*s)) s++;

  if (!*s)
  {
    for (int i = 0; i < NUM_CMDS; i++)
      sim_callback->printf_filtered (sim_callback, "%s -- %s\n", sim_cmd_table[i].name, sim_cmd_table[i].usage);
  }
  else
  {
    int c = sim_cmd_lookup (s, 1);
    if (c < 0)
      sim_callback->printf_filtered (sim_callback, "unknown command \"%s\"\n", s);
    else if (c >= NUM_CMDS)
      sim_callback->printf_filtered (sim_callback, "ambiguous command \"%s\"\n", s);
    else
      sim_callback->printf_filtered (sim_callback, "%s  %s\n", sim_cmd_table [c].usage, sim_cmd_table [c].full);
  }
}

static void sim_cmd_connect (char*s)
{
  char host [200];
  int port;

  while (*s && isspace (*s)) s++;
  
  int num_cpus = 1;
  int cpu = 0;
  
  if (sscanf (s, "%s %d", host, &port) == 2 ||
      sscanf (s, "%s %d %d", host, &port, &cpu) == 3 ||
      sscanf (s, "%s %d %d %d", host, &port, &cpu, &num_cpus) == 4)
  {
    try
    {
      insist (num_cpus > 0 && num_cpus <= 2);
      if (!client.connect (host, port, cpu, num_cpus))
      {
	sim_callback->printf_filtered (sim_callback, "couldn't connect to %s:%d.\n", host, port);
	return;
      }
      
      sim_callback->printf_filtered (sim_callback, "Connected to gdb_server.\n");
    }
    catch (exception_t&e)
    {
      e.print ();
      return;
    }
  }
  else
  {
    sim_callback->printf_filtered (sim_callback, "huh? say \"connect <host> <port> [<cpu> [num_cpus]]\"\n");
    return;
  } 
}

/*segfault if pc is ever outside of text*/

static void sim_cmd_watch_pc (char*s)
{
  if (!prog_bfd)
  {
    sim_callback->printf_filtered (sim_callback, "no text sections yet, load program first\n");
    return;
  }
  try
  {
    /*tell gdb_server each of the code section offsets and sizes*/
    for (bfd_section*sec = prog_bfd->sections; sec; sec = sec->next)
    {
      if (sec->flags & SEC_CODE)
	client.code ((unsigned) sec->vma, (unsigned) sec->size);
    }
  }
  catch (exception_t&e)
  {
    e.print ();
    return;
  }
}

static void sim_cmd_detach (char*s)
{
  try
  {
    client.detach ();
    sim_callback->printf_filtered (sim_callback, "detached from verilog\n");
  }
  catch (exception_t&e)
  {
    e.print ();
    return;
  }
}

static void sim_cmd_cycle (char*s)
{
  try
  {
    client.resume (1);
    client.wait ();
  }
  catch (exception_t&e)
  {
    e.print ();
    return;
  }
}

int sim_insert_breakpoint (unsigned address)
{
  client.insert_breakpoint (address);
  return 1;
}

int sim_remove_breakpoint (unsigned address)
{
  client.remove_breakpoint (address);
  return 1;
}

