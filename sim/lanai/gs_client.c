#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "gs_client.h"
#include "insist.h"

/* Like read(), but retry instead of returning partial reads, and
   perror on error. */

static
ssize_t
_read (int fd, void *buf, size_t count)
{
  ssize_t red = 0;
  while (count)
    {
      int r = read (fd, buf, count);
      if (-1 == r)
	{
	  if (errno == EINTR || errno == EAGAIN)
	    continue;
	  perror ("my_read");
	  return -1;
	}
      buf = (char *)buf + r;
      count -= r;
      red += r;
    }
  return red;
}
#define READ(f,b,c) insist (-1 != _read (f, b, c))

/* Like write(), but retry instead of returning partial writes, and
   perror on error. */

static
ssize_t
_write (int fd, void *buf, size_t count)
{
  ssize_t red = 0;
  while (count)
    {
      int r = write (fd, buf, count);
      if (-1 == r)
	{
	  if (errno == EINTR || errno == EAGAIN)
	    continue;
	  perror ("my_write");
	  return -1;
	}
      buf = (char *)buf + r;
      count -= r;
      red += r;
    }
  return red;
}
#define WRITE(f,b,c) insist (-1 != _write (f, b, c))

gs_client_t::gs_client_t ()
{
  fd = 0;  
}

gs_client_t::~gs_client_t ()
{
  if (fd) close (fd);
}

int gs_client_t::connect (char*hostname, int port, int cpu, int num_cpus)
{
  insist (this && hostname && port);

  /*connect to the gdb_server*/
  struct sockaddr_in sin;
  struct hostent *he;
  
  memset (&sin, 0, sizeof (sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons (port);

  if ((he = gethostbyname (hostname)))
    memcpy (&sin.sin_addr.s_addr, he->h_addr, he->h_length);
  else if ((sin.sin_addr.s_addr = inet_addr (hostname)) == (unsigned) -1)
    return 0;
  
  if ((fd = socket (PF_INET, SOCK_STREAM, 0)) < 0)
  {
    return 0;
  }  
  
  int option = 1;  
  setsockopt (fd, IPPROTO_TCP, TCP_NODELAY, (char*) &option, sizeof (option));

  if (::connect (fd, (struct sockaddr*) &sin, sizeof (sin)) < 0)
  {
    close (fd);
    return 0;
  }

  /*make the greeting packet*/
  gs_header_t h;
  h.type = GS_HELLO;
  h.num_cpus = num_cpus;
  h.cpu_num = cpu;
  gs_flip_header (h);

  WRITE (fd, &h, sizeof (h));

  /*wait for the ack*/
  wait_for_ack ();
  return 1;
}


/*wait for something to happen on the network.
  return values
  0 means nothing happened.
  < 0 means it was a signal
  > 0 is BREAK or SEGFAULT
*/

int gs_client_t::wait ()
{
  gs_header_t h;

  insist (this && fd > 0);
  
  fd_set set;
  FD_ZERO (&set);
  FD_SET (fd, &set);
  
  int r = select (fd + 1, &set, 0, 0, 0);
  if (!r) return 0; /*nothing happened*/

  if (r < 0)
  {
    /*it's a signal, the user wants to interrupt the process on the other end.*/
    return -1;
  }
  
  /*we have something legit coming in. process it. it should only be a break (since we aren't modeling aborts)*/

  /*read the packet*/
  READ (fd, &h, sizeof (h));
  gs_flip_header (h);
  
  insist (h.type == GS_BREAK || h.type == GS_SEGFAULT);
  return h.type == GS_BREAK ? BREAK : SEGFAULT;
}

/*wait for something to happen on the network.
  return values
  0 means nothing happened.
  < 0 means it was a signal
  > 0 means the other end hit a breakpoint
*/

int gs_client_t::poll ()
{
  gs_header_t h;

  insist (this && fd > 0);
  
  /*make a nice timeval for polling*/
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 0;

  fd_set set;
  FD_ZERO (&set);
  FD_SET (fd, &set);
  
  int r = select (fd + 1, &set, 0, 0, &tv);
  if (!r) return 0; /*nothing happened*/

  if (r < 0)
  {
    /*it's a signal, the user wants to interrupt the process on the other end. this might never happen.*/
    return -1;
  }
  
  /*we have something legit coming in. process it. it should only be a break (since we aren't modeling aborts)*/

  /*read the packet*/
  READ (fd, &h, sizeof (h));
  gs_flip_header (h);
  
  insist (h.type == GS_BREAK);
  return 1;
}


/*we are expecting an ack. just block for it*/
void gs_client_t::wait_for_ack ()
{
  insist (fd > 0);
  
  gs_header_t h;

  /*read the packet*/
  READ (fd, &h, sizeof (h));
  gs_flip_header (h);
  
  /*save what we care from it*/
  if (h.type != GS_ACK)
  {
    printf ("h.type is %d\n", h.type);
  }
  insist (h.type == GS_ACK)
}

unsigned gs_client_t::fetch_register (int regno)
{
  gs_header_t h;

  insist (fd > 0);

  /*make the read register packet*/
  h.type = GS_READ_REGISTER;
  h.regno = regno;
  gs_flip_header (h);

  /*send it*/
  WRITE (fd, &h, sizeof (h));

  /*get the answer*/
  READ (fd, &h, sizeof (h));
  gs_flip_header (h);
  
  /*save what we care from it*/
  if (h.type != GS_READ_REGISTER_ACK)
  {
    printf ("h.type is %d\n", h.type);
  }
  
  insist (h.type == GS_READ_REGISTER_ACK); 
  return h.word;
}

void gs_client_t::store_register (int regno, unsigned value)
{
  insist (this && fd > 0);
  gs_header_t h;

  /*make the write register packet*/
  h.type = GS_WRITE_REGISTER;
  h.regno = regno;
  h.word = value;
  
  gs_flip_header (h);

  /*send it*/
  WRITE (fd, &h, sizeof (h));

  wait_for_ack ();
}

void gs_client_t::read_memory (unsigned address, unsigned length, char*buffer)
{
  insist (this && fd > 0);
  gs_header_t h;

  /*make the read packet*/
  h.type = GS_READ_MEMORY;
  h.length = length;
  h.address = address;
  
  gs_flip_header (h);

  /*send it*/
  WRITE (fd, &h, sizeof (h));

  /*get the header of the read response*/
  READ (fd, &h, sizeof (h));
  gs_flip_header (h);
  
  //printf ("h.type is %d, h.length is %d\n", h.type, h.length);
  insist (h.type == GS_READ_MEMORY_ACK && h.length == length);
  
  /*now the data*/
  READ (fd, buffer, length);
}

void gs_client_t::write_memory (unsigned address, unsigned length, char*buffer)
{
  insist (this && fd > 0);
  gs_header_t h;

  /*make the write packet*/
  h.type = GS_WRITE_MEMORY;
  h.length = length;
  h.address = address;

  gs_flip_header (h);

  /*send it, first the header, then the data*/
  WRITE (fd, &h, sizeof (h));
  WRITE (fd, buffer, length);

  wait_for_ack ();
}


void gs_client_t::_break ()
{
  //printf ("_break\n");
  send_and_wait (GS_BREAK);
}

void gs_client_t::detach ()
{
  send_and_wait (GS_DETACH);
}

void gs_client_t::resume (int step)
{
  insist (fd > 0);
  
  gs_header_t h;
  h.type = GS_RESUME;
  h.step = step;
  gs_flip_header (h);
  WRITE (fd, &h, sizeof (h));

  wait_for_ack ();
}

/*send a simple packet, just the type matters*/
void gs_client_t::send_and_wait (int type)
{
  insist (fd > 0);
  
  gs_header_t h;
  h.type = type;
  gs_flip_header (h);
  WRITE (fd, &h, sizeof (h));

  wait_for_ack ();
}



void gs_client_t::code (unsigned address, unsigned length)
{
  insist (fd > 0);
  
  gs_header_t h;
  h.type = GS_CODE;
  h.address = address;
  h.length = length;
  
  gs_flip_header (h);
  WRITE (fd, &h, sizeof (h));

  wait_for_ack ();
}

void gs_client_t::insert_breakpoint (unsigned address)
{
 insist (fd > 0);
  
  gs_header_t h;
  h.type = GS_INSERT_BREAKPOINT;
  h.address = address;
  
  gs_flip_header (h);
  WRITE (fd, &h, sizeof (h));

  wait_for_ack ();
}

void gs_client_t::remove_breakpoint (unsigned address)
{
 insist (fd > 0);
  
  gs_header_t h;
  h.type = GS_REMOVE_BREAKPOINT;
  h.address = address;
  
  gs_flip_header (h);
  WRITE (fd, &h, sizeof (h));

  wait_for_ack ();
}
