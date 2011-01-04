#include <errno.h>
#include <unistd.h>
#include <stdio.h>

#include "better.h"

/* Like write() but retry on EINTR and EAGAIN.
   Always return the number of bytes requested but *NOT* read.
   On success, returns zero.
   On error, returns nonzero and sets errno.
   On EOF, returns nonzero and sets errno to EAGAIN.*/

unsigned long
better_write (int fd, const void *p, unsigned long count) 
{
  while (count) {
    long r = write (fd, p, count);
    if (r == -1) {
      if (errno == EINTR || errno == EAGAIN)
	continue;
      perror ("better_write()");
      break;
    }
    if (r == 0) {
      errno = EAGAIN;
      break;
    }
    count -= r;
    p = (char *)p + r;
  }
  return count;
}
