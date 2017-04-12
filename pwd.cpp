#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
/**
this program returns the path in which
the user is in currently using the
pathconf and getcwd fucntions
 */
int main(int argc, char * argv[]){
  int size;
  char* str;
  char * ptr;
  size =pathconf(".", _PC_PATH_MAX);
  str= argv[1];
  ptr= getcwd(str, (size_t)size);
  printf("%s\n", ptr);
}
