#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

using namespace std;

bool mkdir_norm(char *);
bool mkdir_parents(char *);

bool create_intermediates;
bool set_mode;
char * new_mode;

int main(int argc, char * argv[]) {
    int optc;
    while ((optc = getopt(argc, argv, "pm:")) != -1) {
        switch (optc) {
            case 'p':
                create_intermediates = true;
                break;
            case 'm':
                set_mode = true;
                new_mode = optarg;
                break;
        }
    }

    for (int i = optind; i < argc; i++) {
        if (!create_intermediates) {
            if (!mkdir_norm(argv[i]))
                exit(EXIT_FAILURE);
        } else {
            if (!mkdir_parents(argv[i]))
                exit(EXIT_FAILURE);
            if (!mkdir_norm(argv[i]))
                exit(EXIT_FAILURE);
        }
    }
}

bool mkdir_norm(char * dir) {
    mode_t mode;
    if (!set_mode) {
        mode = (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    } else {
        mode = stoul(new_mode, NULL, 8);
        umask(0);
    }

    if (mkdir(dir, mode) != 0) {
        perror("mkdir");
        return false;
    }

    return true;
}

bool mkdir_parents(char * dir) {
    char tmp[256];
    char *ptr;

    snprintf(tmp, sizeof(tmp), "%s", dir);
    int len = strlen(tmp);

    if (tmp[len-1] == '/')
        tmp[len-1] = 0;

    for (ptr = tmp+1; *ptr; ptr++) {
        if (*ptr == '/') {
            *ptr = 0;
            if (mkdir(tmp, 0755) != 0) {
                perror("mkdir");
                return false;
            }
            *ptr = '/';
        }
    }

    return true;
}
