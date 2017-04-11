#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <stdio.h>
int main(int argc, char* argv[]){
  int lnk;
  char home[] = "/home";
  if(argc ==1){
    printf("%s: missing file operand\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  else if(argc ==2){    
    lnk= link(argv[1],home);
  }
  else if(argc>3){
    char* str= argv[1];
    if(getopt(argc,argv,"-s")){
      //printf("
      lnk =symlink(argv[2],argv[3]);
      if(lnk==-1){
	perror("ln");
	exit(EXIT_FAILURE);
      }
    }
  }
  else{
    lnk=link(argv[1],argv[2]);
    if(lnk==-1){
      perror("ln");
      exit(EXIT_FAILURE);
    }
  }
}
