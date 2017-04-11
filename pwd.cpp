#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
  int size;
  char* str;
  char * ptr;
  size =pathconf(".", _PC_PATH_MAX);
  str= argv[1];
  ptr= getcwd(str, (size_t)size);
  printf("Current Directory is in: %s\n", ptr);
}
