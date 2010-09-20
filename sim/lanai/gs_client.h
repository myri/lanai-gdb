#include "gs.h"
#include "sm_literate.h"

class gs_client_t : public sm_literate_t
{
  public:
  enum
  {
    BREAK = 1,
    SEGFAULT
  };
  
  private:
  int fd;
  void wait_for_ack ();
  void send_and_wait (int type);

  public:
  gs_client_t ();
  ~gs_client_t ();
  int connect (char*hostname, int port, int cpu, int num_cpus);
  unsigned fetch_register (int regno);
  void store_register (int regno, unsigned value);
  void read_memory (unsigned address, unsigned length, char*buffer);
  void write_memory (unsigned address, unsigned length, char*buffer);
  void _break ();
  void code (unsigned address, unsigned length);
  void resume (int step);
  int wait ();  
  int poll ();  
};
  
