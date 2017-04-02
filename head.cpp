#include <cstdlib>
#include <iostream>
#include <fcntl.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

using std::cout;
using std::string;

int max_lines = 10;

int main (int argc, char * argv[]) {
    char * fname;
    int fd;

    int lines_left;

    const size_t BUFF_SIZE = 1024;
    char buffer [BUFF_SIZE];
    int n;

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (string(argv[i]) == "-n") {
                max_lines = atoi(argv[i+1]);
            } else {
                fname = argv[i];
                if ((fd = open(fname, O_RDONLY)) != -1) {
                    lines_left = max_lines;
                    n = 0;
                    while (lines_left) {
                        n = read(fd, buffer, BUFF_SIZE);
                        if (n == 0) break;
                        for (int i = 0; i < n; i++) {
                            if (buffer[i] == '\n' && lines_left-- == 0) {
                                off_t bytes_past_EOL = n - i;
                                if (lseek (fd, -bytes_past_EOL, SEEK_CUR) < 0) {
                                    //struct stat st;
                                    //if (fstat (fd, &st) != 0 || S_ISREG (st.st_mode))
                                        //elseek (fd, -bytes_past_EOL, SEEK_CUR, fname);
                                }
                                break;
                            }
                        }
                        if (write(STDOUT_FILENO, buffer, n) == -1) {
                            cout << "Error while writing file to stdout.";
                            exit(EXIT_FAILURE);
                        }
                    }
                    close(fd);
                } else {
                    cout << "Error while opening file.";
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
