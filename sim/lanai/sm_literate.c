#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "insist.h"
#include "sm_literate.h"

int sm_literate_t::read (int fd, char*buffer, int length)
{
  int total;

  insist (this);
  insist (fd && buffer && length > 0);

  total = my_read (fd, buffer, length, 0);
  if (total < length)
    return 0;
  return total;
}

int sm_literate_t::write (int fd, char*buffer, int length)
{
  int total, n;

  insist (this);
  insist (fd && buffer && length > 0);
  
  for (total = 0; total < length; total += n)
  {
    if ((n = ::write (fd, buffer + total, length - total)) < 1)
      return 0;
    //insist (n == total);
  }
  return total;
}



/* Read the specified number of bytes, automatically retrying the
   read on nonfatal errors.

   Returns the number of bytes read.

   If EOF causes a partial read, 0 is stored at *error.
   If an error causes a partial read, errno is stored at *error. */

int sm_literate_t::my_read (int fd, void *p, int count, int *error /* optional */) 
{
  ssize_t r;
  size_t red=0;

  for (red=0; red<count; red+=(size_t)r,p=(char *)p+r) {
    
    /* Perform a read, ignoring interruptions due to signals and
       nonblocking socket/file settings. */
    
    do {
      r = ::read (fd, (char *)p + red, count - red);
    } while (r == -1 && (errno == EINTR || errno == EAGAIN));

    /* Return on error */
    
    if (r == -1) {
      printf ("%s(): %s\n", __FUNCTION__, strerror (errno));
      if (error)
	*error = errno;
      return red;
    }

    /* Return on EOF. */
    
    if (r == 0) {
      // printf ("%s(): EOF\n", __FUNCTION__);
      if (error)
	*error = 0;
      return red;
    }
  }
  return red;
}
