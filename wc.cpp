#include <iostream>
#include <iomanip>
#include <vector>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <cwchar>
#include <string>

using namespace std;

void count(int, int);
int calc_maxw();
void print_counts(int, int, int, int);

bool print_bytes;
bool print_lines;
bool print_chars;
bool print_words;

const size_t BUFF_SIZE = 1024;

int total_lines, total_words, total_chars, total_bytes;

int * line_ary;
int * word_ary;
int * char_ary;
int * byte_ary;
uint maxw;

vector<char *> fileargs;

/**
 * Parses program arguments and processes them
 */
int main(int argc, char * argv[]) {
    // parses arguments given to program
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            // if argument is an option
            if ((argv[i][0] == '-') && (strcmp(argv[i], "-") != 0)) {
                for (uint j = 1; j < strlen(argv[i]); j++) {
                    switch (argv[i][j]) {
                        case 'c':
                            print_bytes = true;
                            break;
                        case 'm':
                            print_chars = true;
                            break;
                        case 'l':
                            print_lines = true;
                            break;
                        case 'w':
                            print_words = true;
                            break;
                    }
                }
            }
            // if argument is file
            else {
                fileargs.push_back(argv[i]);
            }
        }
    }

    // allocate array for each count to determine max width of values
    int files = fileargs.size();
    line_ary = new int[files];
    word_ary = new int[files];
    char_ary = new int[files];
    byte_ary = new int[files];

    bool * succeed = new bool[files];
    if (files > 0) {
        // for each file, attempt to open and count
        for (int i = 0; i < files; i++) {
            if (strcmp(fileargs[i], "-") != 0) {
                int fd;
                if ((fd = open(fileargs[i], O_RDONLY)) != -1) {
                    count(fd, i);
                    succeed[i] = true;
                } else {
                    string msg = ("wc: " + string(fileargs[i]));
                    perror(msg.c_str());
                    succeed[i] = false;
                }
            } else {
                count(STDIN_FILENO, i);
                succeed[i] = true;
            }
        }
        // calculate max width of all counts
        maxw = calc_maxw();
        // for each file, print counts if count succeeded
        for (int i = 0; i < files; i++) {
            if (succeed[i]) {
                print_counts(line_ary[i], word_ary[i], char_ary[i], byte_ary[i]);
                cout << fileargs[i] << endl;
            }
        }
        // if multiple file arguments given, print total counts at the end
        if (files > 1) {
            print_counts(total_lines, total_words, total_chars, total_bytes);
            cout << "total" << endl;
        }
    } 
    // if no arguments given, use stdin
    else {
        count(STDIN_FILENO, 0);
        maxw = calc_maxw();
        print_counts(line_ary[0], word_ary[0], char_ary[0], byte_ary[0]);
        cout << endl;
    }

    // deallocate arrays
    delete[] line_ary;
    delete[] word_ary;
    delete[] char_ary;
    delete[] byte_ary;
    delete[] succeed;
}

/**
 * Count number of bytes, words, lines, and characters in each file and add to
 * respective array at findex
 */
void count(int fd, int findex) {
    char buffer[BUFF_SIZE];
    int n;
    std::mbstate_t state = std::mbstate_t();

    int bytes = 0;
    int lines = 0;
    int chars = 0;
    int words = 0;

    n = 0;
    bool word = false;
    while ((n = read(fd, buffer, BUFF_SIZE)) > 0) {
        // BYTES
        bytes += n;

        // WORDS
        for (int i = 0; i < n; i++) {
            if (isprint(buffer[i]) && buffer[i] != ' ') {
                word = true;
            }
            if (buffer[i] == ' ' || buffer[i] == '\n') {
                words += word;
                word = false;
            }
        }

        // LINES
        for (int i = 0; i < n; i++) {
            if (buffer[i] == '\n') {
                lines++;
            }
        }

        // CHARACTERS
        const char * ptr = buffer;
        int bytes_left = n;
        while (bytes_left > 0) {
            wchar_t wc;
            int len;

            // get length of char (to handle wide chars)
            len = mbrtowc(&wc, ptr, bytes_left, &state);
            // if error, increment pointer and continue
            if (len == -1) {
                ptr++;
                continue;
            }
            // if length is 0, set len to 1 to continue
            if (len == 0) {
                len = 1;
            }

            // increment pointer by len for next char
            ptr += len;
            bytes_left -= len;
            // increment char count
            chars++;
        }
    }
    words += word;

    // add counts to totals
    total_lines += lines;
    total_words += words;
    total_chars += chars;
    total_bytes += bytes;

    // add count to array at findex
    line_ary[findex] = lines;
    word_ary[findex] = words;
    char_ary[findex] = chars;
    byte_ary[findex] = bytes;
}

/**
 * Calculates the maximum width of all counts for formatting
 */
int calc_maxw() {
    uint maxw = 0;
    int files = fileargs.size();

    // for each array entry/file
    for (int i = 0; i < files; i++) {
        // if no options given, print default counts
        if (!print_bytes && !print_lines && !print_chars && !print_words) {
            if (to_string(line_ary[i]).length() > maxw) maxw = to_string(line_ary[i]).length();
            if (to_string(word_ary[i]).length() > maxw) maxw = to_string(word_ary[i]).length();
            if (to_string(char_ary[i]).length() > maxw) maxw = to_string(char_ary[i]).length();
        }
        // otherwise test only given options for width
        else {
            if ((to_string(line_ary[i]).length() > maxw) && print_lines) maxw = to_string(line_ary[i]).length();
            if ((to_string(word_ary[i]).length() > maxw) && print_words) maxw = to_string(word_ary[i]).length();
            if ((to_string(char_ary[i]).length() > maxw) && print_chars) maxw = to_string(char_ary[i]).length();
            if ((to_string(byte_ary[i]).length() > maxw) && print_bytes) maxw = to_string(byte_ary[i]).length();
        }
    }

    // check width of totals too
    if (files > 1) {
        if (to_string(total_lines).length() > maxw) maxw = to_string(total_lines).length();
        if (to_string(total_words).length() > maxw) maxw = to_string(total_words).length();
        if (to_string(total_chars).length() > maxw) maxw = to_string(total_chars).length();
        if (to_string(total_bytes).length() > maxw) maxw = to_string(total_bytes).length();
    }

    return maxw;
}

/**
 * Prints counts with formatting
 */
void print_counts(int lines, int words, int chars, int bytes) {
    // if no options given, print default counts (lines, words, and bytes)
    if (!print_bytes && !print_lines && !print_chars && !print_words) {
        cout << setw(maxw) << lines << " " << setw(maxw) << words << " " << setw(maxw) << bytes << " ";
    }

    // otherwise print requested counts
    if (print_lines) {
        cout << setw(maxw) << lines << " ";
    }
    if (print_words) {
        cout << setw(maxw) << words << " ";
    }
    if (print_chars) {
        cout << setw(maxw) << chars << " ";
    }
    if (print_bytes) {
        cout << setw(maxw) << chars << " ";
    }
}
