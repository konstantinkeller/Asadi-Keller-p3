#include <iostream>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <fstream>
using namespace std;
int main(int argc, char *argv[])
{
  char outstr[100];
  
  time_t t;
  struct tm *tmp;
  //  char* chtime = ctime(&t);
  //cout<< chtime;
  t = time(NULL);
  tmp= localtime(&t);
  
  if (tmp == NULL) {
    perror("localtime");
    exit(EXIT_FAILURE);
  }
  if(argc==1){
    strftime(outstr, sizeof(outstr), "%a %b %e %H:%M:%S %Z %Y", tmp);
    printf("%s\n", outstr);
    exit(EXIT_SUCCESS);
  }
  else{
    char *str = argv[1];
    if(str[0]=='+'){
      strftime(outstr, sizeof(outstr), argv[1], tmp);
      printf("%.*s\n",100, outstr+1);
      //fprintf(stderr, "strftime returned 0");
      exit(EXIT_SUCCESS);
    }
    else{
      //      perror("write");
      printf("date: invalid date `%s'\n",argv[1]);   
      exit(EXIT_FAILURE);
    }
  }
  //printf("Result string is \"%s\"\n", outstr);
  //exit(EXIT_SUCCESS);
} 
