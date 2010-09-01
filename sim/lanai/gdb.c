#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <bfd.h>
#include <signal.h>
#include "gdb/callback.h"
#include "gdb/remote-sim.h"
#include "ansidecl.h"
#include "sim-utils.h"
#include "run-sim.h"
#include "gdb/signals.h"
#include "../common/sim-utils.h"

static SIM_OPEN_KIND sim_kind;
static char *myname;
host_callback *sim_callback;

SIM_DESC sim_open PARAMS ((SIM_OPEN_KIND kind, struct host_callback_struct *callback, struct bfd *abfd, char **argv))
{
  sim_callback = callback;
  myname = argv [0];
  
  sim_callback->printf_filtered (sim_callback, "sim_open fuck\n");
  //  set_prompt ("(lanai) ");

  /*don't play the bullshit common simulators game*/
  return (SIM_DESC) 1;
}

void sim_close PARAMS ((SIM_DESC sd, int quitting))
{
  sim_callback->printf_filtered (sim_callback, "sim_close\n");
}

SIM_RC sim_load PARAMS ((SIM_DESC sd, char *prog, struct bfd *abfd, int from_tty))
{
  sim_callback->printf_filtered (sim_callback, "sim_load\n");

  bfd *prog_bfd;

  prog_bfd = sim_load_file (sd, myname, sim_callback, prog, abfd,
			    sim_kind == SIM_OPEN_DEBUG, 0, sim_write);
  if (prog_bfd == NULL)
    return SIM_RC_FAIL;
  //  ARMul_SetPC (state, bfd_get_start_address (prog_bfd));
  if (abfd == NULL)
    bfd_close (prog_bfd);

  return SIM_RC_OK;
}

SIM_RC sim_create_inferior PARAMS ((SIM_DESC sd, struct bfd *abfd, char **argv, char **env))
{
  sim_callback->printf_filtered (sim_callback, "sim_create_inferior\n");

  return SIM_RC_OK;
}

int sim_read PARAMS ((SIM_DESC sd, SIM_ADDR mem, unsigned char *buf, int length))
{
  sim_callback->printf_filtered (sim_callback, "sim_read\n");
  return 1;
}

int sim_write PARAMS ((SIM_DESC sd, SIM_ADDR mem, unsigned char *buf, int length))
{
  sim_callback->printf_filtered (sim_callback, "sim_write\n");
  return 1;
}

int sim_fetch_register PARAMS ((SIM_DESC sd, int regno, unsigned char *buf, int length))
{
  sim_callback->printf_filtered (sim_callback, "sim_fetch_register\n");
  return 4;
}

int sim_store_register PARAMS ((SIM_DESC sd, int regno, unsigned char *buf, int length))
{
  sim_callback->printf_filtered (sim_callback, "sim_store_register\n");
  return 4;
}


void sim_resume PARAMS ((SIM_DESC sd, int step, int siggnal))
{
  sim_callback->printf_filtered (sim_callback, "sim_resume\n");
}

int sim_stop PARAMS ((SIM_DESC sd))
{
  sim_callback->printf_filtered (sim_callback, "sim_stop\n");
  return 0;
}

void sim_stop_reason PARAMS ((SIM_DESC sd, enum sim_stop *reason, int *sigrc))
{
  sim_callback->printf_filtered (sim_callback, "sim_stop\n");
}

void sim_do_command PARAMS ((SIM_DESC sd, char *cmd))
{
  sim_callback->printf_filtered (sim_callback, "sim_do_command\n");
}

int sim_target_parse_command_line PARAMS ((int x, char **y))
{
  sim_callback->printf_filtered (sim_callback, "sim_target_parse_command_line\n");
  printf ("sim_target_parse_command_line\n\n");
  return x;
}


void sim_target_display_usage PARAMS ((int help))
{
  sim_callback->printf_filtered (sim_callback, "sim_target_display_usage\n");
}

void sim_set_callbacks PARAMS ((struct host_callback_struct *p))
{
  sim_callback->printf_filtered (sim_callback, "sim_set_callbacks\n");
}

void sim_size PARAMS ((int i))
{
  sim_callback->printf_filtered (sim_callback, "sim_size\n");
}

int sim_trace PARAMS ((SIM_DESC sd))
{
  sim_callback->printf_filtered (sim_callback, "sim_trace\n");
  return 0;
}
void
sim_info PARAMS ((SIM_DESC sd ATTRIBUTE_UNUSED, int verbose ATTRIBUTE_UNUSED))
{
}
