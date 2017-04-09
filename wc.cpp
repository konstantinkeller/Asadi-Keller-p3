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

int main(int argc, char * argv[]) {
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
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
            } else {
                fileargs.push_back(argv[i]);
            }
        }
    }

    int files = fileargs.size();
    line_ary = new int[files];
    word_ary = new int[files];
    char_ary = new int[files];
    byte_ary = new int[files];

    bool * succeed = new bool[files];
    if (files > 0) {
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
        maxw = calc_maxw();
        for (int i = 0; i < files; i++) {
            if (succeed[i]) {
                print_counts(line_ary[i], word_ary[i], char_ary[i], byte_ary[i]);
                cout << fileargs[i] << endl;
            }
        }
        if (files > 1) {
            print_counts(total_lines, total_words, total_chars, total_bytes);
            cout << "total" << endl;
        }
    } else {
        count(STDIN_FILENO, 0);
        maxw = calc_maxw();
        print_counts(line_ary[0], word_ary[0], char_ary[0], byte_ary[0]);
        cout << endl;
    }

    delete[] line_ary;
    delete[] word_ary;
    delete[] char_ary;
    delete[] byte_ary;
    delete[] succeed;
}

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

            len = mbrtowc(&wc, ptr, bytes_left, &state);
            if (len == -1) {
                ptr++;
                continue;
            }
            if (len == 0) {
                len = 1;
            }

            ptr += len;
            bytes_left -= len;
            chars++;
        }
    }
    words += word;

    total_lines += lines;
    total_words += words;
    total_chars += chars;
    total_bytes += bytes;

    line_ary[findex] = lines;
    word_ary[findex] = words;
    char_ary[findex] = chars;
    byte_ary[findex] = bytes;
}

int calc_maxw() {
    uint maxw = 0;
    int files = fileargs.size();

    for (int i = 0; i < files; i++) {
        if (!print_bytes && !print_lines && !print_chars && !print_words) {
            if (to_string(line_ary[i]).length() > maxw) maxw = to_string(line_ary[i]).length();
            if (to_string(word_ary[i]).length() > maxw) maxw = to_string(word_ary[i]).length();
            if (to_string(char_ary[i]).length() > maxw) maxw = to_string(char_ary[i]).length();
        } else {
            if ((to_string(line_ary[i]).length() > maxw) && print_lines) maxw = to_string(line_ary[i]).length();
            if ((to_string(word_ary[i]).length() > maxw) && print_words) maxw = to_string(word_ary[i]).length();
            if ((to_string(char_ary[i]).length() > maxw) && print_chars) maxw = to_string(char_ary[i]).length();
            if ((to_string(byte_ary[i]).length() > maxw) && print_bytes) maxw = to_string(byte_ary[i]).length();
        }
    }

    if (files > 1) {
        if (to_string(total_lines).length() > maxw) maxw = to_string(total_lines).length();
        if (to_string(total_words).length() > maxw) maxw = to_string(total_words).length();
        if (to_string(total_chars).length() > maxw) maxw = to_string(total_chars).length();
        if (to_string(total_bytes).length() > maxw) maxw = to_string(total_bytes).length();
    }

    return maxw;
}

void print_counts(int lines, int words, int chars, int bytes) {
    if (!print_bytes && !print_lines && !print_chars && !print_words) {
        cout << setw(maxw) << lines << " " << setw(maxw) << words << " " << setw(maxw) << bytes << " ";
    }

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
