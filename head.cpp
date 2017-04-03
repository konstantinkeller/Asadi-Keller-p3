#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

using namespace std;

void print_lines_file(char *);
void print_lines_stdin();

int max_lines = 10;

int main(int argc, char * argv[]) {
    char * fname;
    vector<char *> fileargs;

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-') {
                switch (argv[i][1]) {
                    case 'n':
                        max_lines = atoi(argv[i+1]);
                }
            } else if (argv[i-1][0] != '-') {
                fname = argv[i];
                fileargs.push_back(fname);
            }
        }
    }

    if (fileargs.size() == 0) {
        print_lines_stdin();
    } else {
        for (char * file : fileargs) {
            print_lines_file(file);
        }
    }

    return EXIT_SUCCESS;
}

void print_lines_file(char * fname) {
    int fd;
    int lines_left;

    int bytes_to_write;

    const size_t BUFF_SIZE = 1024;
    char buffer [BUFF_SIZE];
    int n;

    if ((fd = open(fname, O_RDONLY)) != -1) {
        lines_left = max_lines;
        n = 0;
        while (lines_left > 0) {
            n = read(fd, buffer, BUFF_SIZE);
            if (n == 0) break;
            for (int i = 0; i < n; i++) {
                if (buffer[i] == '\n') {
                    lines_left--;
                    if (lines_left == 0) {
                        bytes_to_write = i;
                        break;
                    }
                }
                bytes_to_write = n;
            }
            if (write(STDOUT_FILENO, buffer, bytes_to_write) == -1) {
                cout << "Error while writing file to stdout.";
                exit(EXIT_FAILURE);
            }
        }
        cout << std::endl;
        close(fd);
    } else {
        cout << "Error while opening file.";
        exit(EXIT_FAILURE);
    }
}

void print_lines_stdin() {
    int lines_left;

    int bytes_to_write;

    const size_t BUFF_SIZE = 1024;
    char buffer [BUFF_SIZE];
    int n;

    lines_left = max_lines;
    n = 0;
    while (lines_left > 0) {
        n = read(STDOUT_FILENO, buffer, BUFF_SIZE);
        if (n == 0) break;
        for (int i = 0; i < n; i++) {
            if (buffer[i] == '\n') {
                lines_left--;
                if (lines_left == 0) {
                    bytes_to_write = i;
                    break;
                }
            }
            bytes_to_write = n;
        }
        if (write(STDOUT_FILENO, buffer, bytes_to_write) == -1) {
            cout << "Error while writing to stdout.";
            exit(EXIT_FAILURE);
        }
    }
    cout << std::endl;
}
