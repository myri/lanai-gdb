#ifndef _better_h_
#define _better_h_

/* Like read() and write() but retry on EINTR and EAGAIN.
   On error, return truncated length and set errno.
   On EOF, return truncated length and set errno to EAGAIN. */

unsigned long better_read (int fd, void *p, unsigned long count);
unsigned long better_write (int fd, const void *p, unsigned long count);

#endif /* _better_h_ */
