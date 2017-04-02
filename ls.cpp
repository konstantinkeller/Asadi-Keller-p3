#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <vector>
#include <algorithm>
#include <locale>
#include <cstring>
#include <string>
#include <unistd.h>

using namespace std;

void sort_files();
void sort_args();
bool comp_ignorecase(string, string);
bool comp_sortbytype(string, string);
void print_list_simple();
void print_list_detailed();

const char THIS_DIR = '.';
const char * THIS_DIR_PTR = &THIS_DIR;

char * current_dir;

vector<string> fileargs;
bool current_arg_is_file;
vector<string> dir_structure;
int fcount;

bool list_all = false;
bool list_detailed = false;

int main(int argc, char * argv[]) {
    DIR * dir;
    struct dirent * dptr;

    if ((current_dir = getenv("PWD")) == NULL) {
        perror("getenv");
        exit(EXIT_FAILURE);
    }
    
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-') {
                for (uint j = 0; j < strlen(argv[i]); j++) {
                    switch (argv[i][j]) {
                        case 'a':
                            list_all = true;
                            break;
                        case 'l':
                            list_detailed = true;
                            break;
                    }
                }
            } else {
                fileargs.push_back(argv[i]);
            }
        }
    }

    if (fileargs.size() == 0) {
        fileargs.push_back(current_dir);
    }

    sort_args();

    for (uint i = 0; i < fileargs.size(); i++) {
        fcount = 0;
        dir_structure.clear();

        chdir(current_dir);
        if (chdir(fileargs[i].c_str()) != -1) {
            current_arg_is_file = false;
            if ((dir = opendir(".")) == NULL) {
                perror(("opendir" + fileargs[i]).c_str());
                exit(EXIT_FAILURE);
            }
            while ((dptr = readdir(dir)) != NULL) {
                if (list_all || !(dptr->d_name[0] == '.')) {
                    dir_structure.push_back(string(dptr->d_name));
                    fcount++;
                } 
            }
            closedir(dir);
        } else {
            current_arg_is_file = true;
            dir_structure.push_back(fileargs[i]);
        }


        sort_files();

        if (i > 0)
            cout << endl;

        if (!list_detailed)
            print_list_simple();
        else
            print_list_detailed();
    }

    exit(EXIT_SUCCESS);
}

void sort_files() {
    sort(dir_structure.begin(), dir_structure.end(), comp_ignorecase);
}

void sort_args() {
    sort(fileargs.begin(), fileargs.end(), comp_sortbytype);
}

bool comp_ignorecase(string a, string b) {
    locale loc;

    for (uint i = 0; i < a.length(); i++)
        a[i] = toupper(a[i], loc);
    for (uint i = 0; i < b.length(); i++)
        b[i] = toupper(b[i], loc);

    if (a[0] == '.')
        a.erase(0, 1);
    if (b[0] == '.')
        b.erase(0, 1);

    return a < b;
}

bool comp_sortbytype(string a, string b) {
    locale loc;
    struct stat sb_a;
    struct stat sb_b;

    if ((lstat(a.c_str(), &sb_a) != -1) && (lstat(b.c_str(), &sb_b) != -1)) {
        if (!S_ISDIR(sb_a.st_mode) && S_ISDIR(sb_b.st_mode))
            return true;
        else if (S_ISDIR(sb_a.st_mode) && !S_ISDIR(sb_b.st_mode))
            return false;
        else {
            for (uint i = 0; i < a.length(); i++)
                a[i] = toupper(a[i], loc);
            for (uint i = 0; i < b.length(); i++)
                b[i] = toupper(b[i], loc);

            return a < b;
        }
    } else {
        perror("comparison lstat");
        exit(EXIT_FAILURE);
    }
}

void print_list_simple() {
    cout.setf(ios::unitbuf);
    for (string file : dir_structure) {
        cout << file << "  ";
    }
    cout << endl;
}

void print_list_detailed() {
    cout.setf(ios::unitbuf);

    int * fsize_ary = new int[fcount];
    int * flink_ary = new int[fcount];
    string * funame_ary = new string[fcount];
    string * fgname_ary = new string[fcount];

    uint max_size_digits = 0;
    uint max_link_digits = 0;
    uint max_uname_chars = 0;
    uint max_gname_chars = 0;
    long long blkcnt = 0;

    struct passwd * pw;
    struct group * gid;

    const char * month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    for (int i = 0; i < fcount; i++) {
        struct stat sb;
        if (lstat(dir_structure[i].c_str(), &sb) != -1) {
            pw = getpwuid(sb.st_uid);
            gid = getgrgid(sb.st_gid);

            fsize_ary[i] = sb.st_size;
            flink_ary[i] = sb.st_nlink;
            funame_ary[i] = string(pw->pw_name);
            fgname_ary[i] = string(gid->gr_name);
            blkcnt += (long long)sb.st_blocks/2;
        }
    }

    for (int i = 0; i < fcount; i++) {
        int size = fsize_ary[i];
        int link = flink_ary[i];
        string uname = funame_ary[i];
        string gname = fgname_ary[i];
        uint size_digits = 0;
        uint link_digits = 0;

        do {
            size_digits++;
            size /= 10;
        } while (size);
        if (size_digits > max_size_digits) max_size_digits = size_digits;

        do {
            link_digits++;
            link /= 10;
        } while (link);
        if (link_digits > max_link_digits) max_link_digits = link_digits;

        if (uname.length() > max_uname_chars) max_uname_chars = uname.length();
        if (gname.length() > max_gname_chars) max_gname_chars = gname.length();
    }
    delete[] fsize_ary;
    delete[] flink_ary;
    delete[] funame_ary;
    delete[] fgname_ary;
    
    if (!current_arg_is_file)
        cout << "total " << blkcnt << endl;
    for (string file : dir_structure) {
        struct stat sb;
        if (lstat(file.c_str(), &sb) != -1) {
            if (S_ISDIR(sb.st_mode))
                cout << "d";
            else if (S_ISBLK(sb.st_mode))
                cout << "b";
            else if (S_ISCHR(sb.st_mode))
                cout << "c";
            else if (S_ISLNK(sb.st_mode))
                cout << "l";
            else if (S_ISFIFO(sb.st_mode))
                cout << "p";
            else if (S_ISSOCK(sb.st_mode))
                cout << "s";
            else cout << "-";
            cout << ((sb.st_mode & S_IRUSR)? "r": "-");
            cout << ((sb.st_mode & S_IWUSR)? "w": "-");
            if (sb.st_mode & S_IXUSR)
                cout << "x";
            else if (sb.st_mode & S_ISUID)
                cout << "S";
            else if (sb.st_mode & S_IXUSR & S_ISUID)
                cout << "s";
            else cout << "-";
            cout << ((sb.st_mode & S_IRGRP)? "r": "-");
            cout << ((sb.st_mode & S_IWGRP)? "w": "-");
            if (sb.st_mode & S_IXGRP)
                cout << "x";
            else if (sb.st_mode & S_ISGID)
                cout << "S";
            else if (sb.st_mode & (S_IXGRP | S_ISGID))
                cout << "s";
            else
                cout << "-";
            cout << ((sb.st_mode & S_IROTH)? "r": "-");
            cout << ((sb.st_mode & S_IWOTH)? "w": "-");
            if ((sb.st_mode & S_IXOTH) && (sb.st_mode & S_ISVTX))
                cout << "t";
            else if (sb.st_mode & S_ISVTX)
                cout << "T";
            else if (sb.st_mode & S_IXOTH)
                cout << "x";
            else
                cout << "-";

            cout << ". ";
            cout << setw(max_link_digits) << setfill(' ') << sb.st_nlink << " ";

            pw = getpwuid(sb.st_uid);
            gid = getgrgid(sb.st_gid);
            cout << setw(max_uname_chars) << setfill(' ') << pw->pw_name << " " << setw(max_gname_chars) << setfill(' ') << gid->gr_name << " ";

            cout << setw(max_size_digits) << setfill(' ') <<  sb.st_size << " ";

            struct tm * timeinfo = localtime(&sb.st_mtime);
            cout << month[timeinfo->tm_mon] << " " << setw(2) << timeinfo->tm_mday << " ";
            cout << setw(2) << setfill('0') << timeinfo->tm_hour << ":" << setw(2) << timeinfo->tm_min << " ";
            if (!S_ISLNK(sb.st_mode)) {
                cout << file;
            } else {
                char link_path[512];
                int count;
                if ((count = readlink(file.c_str(), link_path, sizeof(link_path))) >= 0) {
                    link_path[count] = '\0';
                }
                cout << file << " -> " << link_path;
            }
        }
        cout << endl;
    }
}
