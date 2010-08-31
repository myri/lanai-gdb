#ifndef insist_h
#define insist_h

#include <stdio.h>
#include <errno.h>

class exception_t
{
  private:
  enum
  {
    MAX_STRING = 200
  };
  char s [MAX_STRING];

  public:
  int reason;
  exception_t (const char*fmt, ...);
  exception_t (int reason=0);
  void print ();
  char*get_string ()
  {
    return s;
  }
};

#define insist(e) {;if(!(e)) throw (exception_t ("assertion failed. %s:%d (%s)", __FILE__,__LINE__, #e));}
#define jinsist(e,s) {;if(!(e)) throw (exception_t ("assertion failed. %s:%d (%s)", __FILE__,__LINE__, s));}
#define pinsist(e) if(!(e)) throw (exception_t ("error %s:%d (%s)", __FILE__,__LINE__, strerror (errno)))
#define vinsist(e,s) if(!(e)) throw (exception_t s)
#define error(e) throw (exception_t e)

#endif /*insist_h*/
