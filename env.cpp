#include <iostream>
#include <stdio.h>
/**
This program prints the current set environemental variables
to std output.
 */
int main(int argc, char **argv, char** envp)
{
  char** env;
  //for loop for third parameter limit
  for (env = envp; *env != 0; env++)
    {
      char* thisEnv = *env;
      //printf to string
      printf("%s\n", thisEnv);    
    }
  return(0);
}
