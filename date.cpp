#include <iostream>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <fstream>
using namespace std;
/**
THis program gives the date using
the strftime fucntion and local time
with help from man pages 
 */
int main(int argc, char *argv[])
{
  char outstr[100];  
  time_t t;
  struct tm *tmp;
  //  char* chtime = ctime(&t);
  //cout<< chtime;
  t = time(NULL);
  tmp= localtime(&t);
  //error handling
  if (tmp == NULL) {
    perror("localtime");
    exit(EXIT_FAILURE);
  }
  //one argument will print the string of
  //format set on strftime parameters
  if(argc==1){
    strftime(outstr, sizeof(outstr), "%a %b %e %H:%M:%S %Z %Y", tmp);
    printf("%s\n", outstr);
    exit(EXIT_SUCCESS);
  }
  //checks argument of two with
  //+ and prints the string of argument without +
  else{
    char *str = argv[1];
    if(str[0]=='+'){
      strftime(outstr, sizeof(outstr), argv[1], tmp);
      printf("%.*s\n",100, outstr+1);
      //fprintf(stderr, "strftime returned 0");
      exit(EXIT_SUCCESS);
    }
    //error handling
    else{
      //      perror("write");
      printf("date: invalid date `%s'\n",argv[1]);   
      exit(EXIT_FAILURE);
    }
  }
  //printf("Result string is \"%s\"\n", outstr);
  //exit(EXIT_SUCCESS);
} 
