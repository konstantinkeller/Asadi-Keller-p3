#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <vector>
#include <cstring>

using namespace std;

void print_lines_file(char *);
void print_lines_stdin();

// default number of lines to print
int max_lines = 10;

/**
 * Parses program arguments and runs main program loop
 */
int main(int argc, char * argv[]) {
    char * fname;
    vector<char *> fileargs;

    // parses options
    int optc;
    while ((optc = getopt(argc, argv, "n:")) != -1) {
        switch (optc) {
            case 'n':
                max_lines = atoi(optarg);
                break;
        }
    } 
    
    // parses non-option arguments and adds to filearg vector
    for (int i = optind; i < argc; i++) {
        fname = argv[i];
        fileargs.push_back(fname);
    }

    // unbuffered output
    cout.setf(ios::unitbuf);

    // if no file arguments, use stdin
    if (fileargs.size() == 0) {
        print_lines_stdin();
    } 
    // otherwise use file arguments
    else {
        // for each entry in fileargs vector
        for (uint i = 0; i < fileargs.size(); i++) {
            // print file headers if given multiple arguments
            if (fileargs.size() > 1) {
                cout << "==> " << fileargs[i] << " <==" << endl;
            }
            // if file argument is a hyphen, use stdin
            if (strcmp(fileargs[i], "-") == 0) {
                print_lines_stdin();
            }
            // otherwise process file normally
            else {
                print_lines_file(fileargs[i]);
            }
            if ((fileargs.size() > 1) && (i != fileargs.size() - 1)) {
                cout << endl;
            }
        }
    }

    return EXIT_SUCCESS;
}

/**
 * Prints top max_lines lines for file fname
 */
void print_lines_file(char * fname) {
    int fd;
    int lines_left;

    int bytes_to_write;

    const size_t BUFF_SIZE = 1024;
    char buffer [BUFF_SIZE];
    uint n;

    if ((fd = open(fname, O_RDONLY)) != -1) {
        lines_left = max_lines;
        n = 0;
        while (lines_left > 0) {
            n = read(fd, buffer, BUFF_SIZE);
            if (n == 0) break;
            // check if buffer contains newline char
            for (uint i = 0; i < n; i++) {
                if (buffer[i] == '\n') {
                    // decrement lines_left if newline char is encountered
                    lines_left--;
                    if (lines_left == 0) {
                        // if last line is encountered in buffer, set bytes_to_write to index of newline char
                        bytes_to_write = i;
                        break;
                    }
                }
                // if buffer doesn't contain last line, write entire buffer
                bytes_to_write = n;
            }
            if (n < BUFF_SIZE && buffer[bytes_to_write-1] == '\n') {
                bytes_to_write--;
            }
            if (write(STDOUT_FILENO, buffer, bytes_to_write) == -1) {
                perror("head");
                exit(EXIT_FAILURE);
            }
        }
        cout << std::endl;
        close(fd);
    } else {
        perror("head");
        exit(EXIT_FAILURE);
    }
}

/**
 * Prints top max_lines from stdin input
 */
void print_lines_stdin() {
    int lines_left;

    int bytes_to_write;

    const size_t BUFF_SIZE = 1024;
    char buffer [BUFF_SIZE];
    int n;

    lines_left = max_lines;
    n = 0;
    while (lines_left > 0) {
        // read buffer from stdin
        n = read(STDIN_FILENO, buffer, BUFF_SIZE);
        if (n == 0) break;
        // check if buffer contains newline char
        for (int i = 0; i < n; i++) {
            if (buffer[i] == '\n') {
                // decrement lines_left if newline char is encountered
                lines_left--;
                if (lines_left == 0) {
                    // if last line is encountered in buffer, set bytes_to_write to index of newline char
                    bytes_to_write = i;
                    break;
                }
            }
            // if buffer doesn't contain last line, write entire buffer
            bytes_to_write = n;
        }
        if (write(STDOUT_FILENO, buffer, bytes_to_write) == -1) {
            cout << "Error while writing to stdout.";
            exit(EXIT_FAILURE);
        }
    }
    cout << std::endl;
}
