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
/**
this program will resemble the tail utility in which
it will by default print 10 lines of the last lines in a
file and takes in parameters for cases.
 */
int main(int argc, char * argv[]) {
  int geto;
  char * fname;
  vector<char *> fileargs;
  //operation of getopt for n and c
  while((geto= getopt(argc, argv, "n:c:")) != -1){  
    switch (geto) {
      //n for lines
    case 'n':
      max_lines = atoi(optarg);
      break;
    case 'c':
      //c for characters
      charGive = true;
      max_chars = atoi(optarg);
      break;
    }
  }
  // for loop for arguments
  for(int i = optind; i<argc; i++){
    fname = argv[i];
    fileargs.push_back(fname);
  }

  cout.setf(ios::unitbuf);
  if (fileargs.size() == 0) {
    print_lines_stdin();
  }
  //standard input case
  else {
    for (uint i = 0; i < fileargs.size(); i++) {
      if(strcmp(fileargs[i], "-")==0){
	print_lines_stdin();
	continue;
      }
      //multi argument header
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
/**
This function will calculate the lines and/or characters
of a file
@param fname file name
 */
void print_lines_file(char * fname) {
  //int lines_left;

  //int bytes_to_write;
  vector <char> buff;
  //const size_t BUFF_SIZE = 5024;
  //char buffer [BUFF_SIZE];
  vector <string> twoD;
  int n;
  int file_line= 0;
  char output[1];
  int cur =0;
  int fd;
  //lines_left = max_lines;
  n = 0;
  int c = 0;
  int file_char=0;
  int start=0;

  //opens file of param input
  if ((fd = open(fname, O_RDONLY)) != -1) {
    //reads entire file character by character
    while ((n = read(fd, output, 1)) > 0){
      //pushes to buffer
      buff.push_back(output[0]);
      file_char++;
      if(c == 0) {
        twoD.push_back("");
      } 
      twoD[cur] += output[0];
      c++;
      //line by line
      if(output[0]=='\n'){
        file_line++;
        c=0;
        cur++;
      }
    }
    //    for(int i = 0; i < twoD.size(); i++){
    //cout << twoD.at(i);
    //}
    
    //if c is a true parameter then read characters
    if(charGive == true){
      if(file_char < max_chars){
	start =0;
      }
      //takes last characters
      else{
	start = file_char - max_chars;
      }
      //character by caracter reading into buffer
      for(int i = start; i <= file_char; i++){
	cout << buff [i];
      }
    }
    else{
      if(file_line < max_lines){
	start = 0;
      }
      else
	//takes the remaining lines
	start = file_line - max_lines;
      for(int i = start; i < file_line; i++){
	cout<< twoD[i];
      }
    }
  }
  //  cout << std::endl;
}

/**
function for standard in and prints only last number of lines and/or characters
 */
void print_lines_stdin() {
  //int lines_left;

  //int bytes_to_write;
  vector<char> buff;
  //const size_t BUFF_SIZE = 5024;
  //char buffer [BUFF_SIZE];
  vector<string> twoD;
  int n;
  int file_line= 0;
  char output[1];
  int cur =0;
  //int fd;
  //lines_left = max_lines;
  n = 0;
  int c = 0;
  int file_char=0;
  int start=0;
  //if ((fd = open(fname, O_RDONLY)) != -1) {
  //reads from standard in of file
    while ((n = read(STDIN_FILENO, output, 1)) > 0){
      buff.push_back(output[0]);
      file_char++;
      if( c==0){
	twoD.push_back("");
      }
      twoD [cur] += output[0];
      //reads from end of line
      if(output[0]=='\n'){
        file_line++;
        c=0;
        cur++;
      }
    }
    //character true
    if(charGive == true){
      if(file_char < max_chars){
        start =0;

      }
      
      else{
        start = file_char - max_chars;
      }
      //loops through characters remaining
      for(int i = start; i < file_char; i++){

        cout << buff[i];
      }
    }
    else{
      // restart the position
      if(file_line < max_lines){
        start = 0;
      }
      else
	//remaining lines
        start = file_line - max_lines;

      //loop through remaining lines prints lines
      for(int i = start; i < file_line; i++){
        cout<< twoD[i];
      }
    }
}
