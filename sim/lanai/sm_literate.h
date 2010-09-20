#ifndef sm_literate_h
#define sm_literate_h

class sm_literate_t
{
  private:
  int my_read (int fd, void *p, int count, int *error /* optional */);

  protected:
  int read (int fd, char*buffer, int length);
  int write (int fd, char*buffer, int length);
};

#endif /*sm_literate_h*/
