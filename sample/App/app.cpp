#include "keystone.h"
#include "Enclave_u.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>

EDGE_EXTERNC_BEGIN

unsigned int ocall_print_string(const char* str)
{
  return printf("%s", str);
}

int ocall_open_file(const char* str, int flags, int perm)
{
  printf("[HOST] open %s flags %x\n", str, flags, perm);
  return open(str, flags, perm);
}

int ocall_close_file(int desc)
{
  printf("[HOST] close %d\n", desc);
  return close(desc);
}

int ocall_read_file(int desc, char* buf, unsigned int len)
{
  printf("[HOST] read %d from file\n", len);
  int n = read(desc, buf, len);
  return n;
}

int ocall_write_file(int desc, const char* buf, unsigned int len)
{
  printf("[HOST] write %d to file\n", len);
  int n = write(desc, buf, len);
  return n;
}

EDGE_EXTERNC_END

int main(int argc, char** argv)
{
  Keystone enclave;
  Params params;

  params.setFreeMemSize(2 * 1024 * 1024);
  params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 2 * 1024 * 1024);

  enclave.init(argv[1], argv[2], params);

  enclave.registerOcallDispatch(incoming_call_dispatch);

  /* We must specifically register functions we want to export to the
     enclave. */
  register_functions();
	
  edge_call_init_internals((uintptr_t)enclave.getSharedBuffer(),
			   enclave.getSharedBufferSize());
  enclave.run();

  return 0;
}

