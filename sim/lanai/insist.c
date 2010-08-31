#include "insist.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

exception_t::exception_t (const char*fmt, ...)
{
  va_list args;
  va_start (args, fmt);
  vsprintf (s, fmt, args);
  reason = SIGABRT;
  va_end (args);
}


exception_t::exception_t (int reason)
{
  this->reason = reason;
  *s = 0;
}

void exception_t::print ()
{
  if (*s)
    printf ("%s\n", s);
}
