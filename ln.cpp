#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <stdio.h>
/**
this program will link a file with the link
depending on the arguments. This can also be
a symbolic link
 */
int main(int argc, char* argv[]){
  int lnk;
  char home[] = "/home";
  //error handling
  if(argc ==1){
    printf("%s: missing file operand\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  //linking to base of home path
  else if(argc ==2){    
    lnk= link(argv[1],home);
  }
  //if statment of symbolic or hard link
  else if(argc>3){
    //    char* str= argv[1];
    if(getopt(argc,argv,"-s")){
      //printf("
      //makes a symbolic link of arguments
      lnk =symlink(argv[2],argv[3]);
      if(lnk==-1){
	perror("ln");
	exit(EXIT_FAILURE);
      }
    }
  }
  //makes hard link
  else{
    lnk=link(argv[1],argv[2]);
    if(lnk==-1){
      perror("ln");
      exit(EXIT_FAILURE);
    }
  }
}
