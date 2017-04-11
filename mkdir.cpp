#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

using namespace std;

bool mkdir_norm(char *);
bool mkdir_p(char *);

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
            if (!mkdir_norm(argv[i])) {
                exit(EXIT_FAILURE);
            }
        }
    }
}

bool mkdir_norm(char * dir) {
    mode_t mode;
    if (!set_mode) {
        mode = (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    } else {
        mode = stoul(new_mode, NULL, 8);
        mode_t umask_val = umask(0);
    }

    if (mkdir(dir, mode) != 0) {
        perror("mkdir");
        return false;
    }

    return true;
}

bool mkdir_p(char * dir) {
    struct stat sb;
    char * current_dir = getenv("PWD");

    mode_t mode;
    if (!set_mode) {
        mode = (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    } else {
        mode = stoul(new_mode, NULL, 8);
        mode_t umask_val = umask(0);
    }

    
}
