
#ifndef list_tester_h
#define list_tester_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

void run_test_script(const char* script);
void run_test_folder(const char* path);

#endif /* list_tester_h */
