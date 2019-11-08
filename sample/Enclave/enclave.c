#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"
#include "malloc.h"

#include "Enclave_t.h"

#define O_RDONLY   0
#define O_WRONLY   00001
#define O_CREAT    00100
#define O_TRUNC    01000

void EAPP_ENTRY eapp_entry()
{
  int desc;

  ocall_print_string("Hello world!\n");

  desc = ocall_open_file("FileOne", O_WRONLY|O_CREAT|O_TRUNC, 0644);
  if (desc < 0)
    ocall_print_string("open error!\n");
  else {
    char numstr[3];
    numstr[2] = '\0';
    numstr[1] = '\n';
    numstr[0] = '0' + (desc % 10);
    ocall_print_string(numstr);
  }
  static char buf[] = "0123456789abcdef";
  ocall_write_file(desc, buf, 16);
  ocall_close_file(desc);

  EAPP_RETURN(0);
}
