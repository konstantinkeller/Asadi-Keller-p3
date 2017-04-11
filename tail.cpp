#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>
#include <cstring>
using namespace std;

void print_lines_file(char *);
void print_lines_stdin();

int max_lines = 10;
int max_chars = 10;
bool charGive = false;
int main(int argc, char * argv[]) {
  char * fname;
  vector<char *> fileargs;
  
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
	if(strcmp(argv[i], "-")==0){
	  fname = argv[i];
	  fileargs.push_back(fname);
	  continue;
	}
	switch (argv[i][1]) {
	case 'n':
	  max_lines = atoi(argv[i+1]);
	  break;
	case 'c':
	  charGive = true;
	  max_chars = atoi(argv[i+1]);
	  break;
	}
      } else if(strcmp(argv[i], "-n")!=0 && strcmp(argv[i], "-c")!=0){
	fname = argv[i];
	fileargs.push_back(fname);
      }
    }
  }
  cout.setf(ios::unitbuf);
  if (fileargs.size() == 0) {
    print_lines_stdin();
  }
  else {
    for (uint i = 0; i < fileargs.size(); i++) {
      if(strcmp(fileargs[i], "-")==0){
	print_lines_stdin();
	continue;
      }
  
    if (fileargs.size() > 1) {
	cout << "==> " << fileargs[i] << " <==" << endl;
      }
      print_lines_file(fileargs[i]);
      if ((fileargs.size() > 1) && (i != fileargs.size() - 1)) {
	cout << endl;
      }
    }
  }
  return EXIT_SUCCESS;
}
void print_lines_file(char * fname) {
  int lines_left;

  int bytes_to_write;

  const size_t BUFF_SIZE = 5024;
  char buffer [BUFF_SIZE];
  vector <string> twoD;
  int n;
  int file_line= 0;
  char output[1];
  int cur =0;
  int fd;
  lines_left = max_lines;
  n = 0;
  int c = 0;
  int file_char=0;
  int start=0;
  if ((fd = open(fname, O_RDONLY)) != -1) {

    while ((n = read(fd, output, 1)) > 0){
      if(output[0]=='\n'){
	file_line++;
	c=0;
	cur++;
      }
      buffer [file_char] = output[0];
      file_char++;
      if(c == 0) {
        twoD.push_back("");
      } 
      twoD[cur] += output[0];
      c++;
    }
    if(charGive == true){
      if(file_char < max_chars){
	start =0;

      }

      else{
	start = file_char - max_chars;
      }
      for(int i = start; i < file_char; i++){

	cout << buffer [i];
      }
    }
    else{
      if(file_line < max_lines){
	start = 0;
      }
      else
	start = file_line - max_lines;
      for(int i = start; i < file_line; i++){
	cout<< twoD[i];
      }
    }
  }
  cout << std::endl;
}


void print_lines_stdin() {
  int lines_left;

  int bytes_to_write;

  const size_t BUFF_SIZE = 5024;
  char buffer [BUFF_SIZE];
  char twoD [50][BUFF_SIZE];
  int n;
  int file_line= 0;
  char output[1];
  int cur =0;
  int fd;
  lines_left = max_lines;
  n = 0;
  int c = 0;
  int file_char=0;
  int start=0;
  //if ((fd = open(fname, O_RDONLY)) != -1) {

    while ((n = read(STDIN_FILENO, output, 1)) > 0){
      if(output[0]=='\n'){
        file_line++;
        c=0;
        cur++;
      }
      buffer [file_char] = output[0];
      file_char++;
      twoD[cur][c++]= output[0];

    }
    if(charGive == true){
      if(file_char < max_chars){
        start =0;

      }

      else{
        start = file_char - max_chars;
      }
      for(int i = start; i < file_char; i++){

        cout << buffer [i];
      }
    }
    else{
      if(file_line < max_lines){
        start = 0;
      }
      else
        start = file_line - max_lines;
      for(int i = start; i < file_line; i++){
        cout<< twoD[i];
      }
    }
  cout << std::endl;
}
