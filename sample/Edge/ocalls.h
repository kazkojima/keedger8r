/* Sample edge definitions for keyedge.  */

#include <ocalls_header.h>

// Add edge call function declarations here.

unsigned int ocall_print_string(keyedge_str const char* str);

int ocall_open_file(keyedge_str const char* str, int flags, int perm);
int ocall_close_file(int desc);
int ocall_write_file(int desc, keyedge_vla const char *buf, keyedge_size unsigned int len);
