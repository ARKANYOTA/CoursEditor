//
// Created by ay on 14/11/2021.
//
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <iostream>     // std::cout
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <set>
#include <ctime>
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <cstring>
#include <json/json.h>
#include <fstream>

using namespace std;
#ifndef CPP_USEFULL_FUNCTION_H
#define CPP_USEFULL_FUNCTION_H

const string borderutf8 = "\u2501";
const string space = " ";

string operator*(string a, unsigned int b) {
    string output = "";
    while (b--) {
        output += a;
    }
    return output;
}
string convertToString(char *a, int size) {
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}
void clear_screen() {
    cout << "\033[2J\033[1;1H";
}
bool path_is_dir(filesystem::path path) {
    return filesystem::is_directory(path);
}
int string_length(string s) {
    int ssize = 0;
    bool is_accent = false;
    bool is_esc = false;
    for (int i = 0; i < s.size(); i++) {
        if (is_esc) {
            if (s[i] == 'm' or s[i] == 'H') {
                is_esc = false;
                continue;
            }
        } else {
            if (int(s[i]) < 0 and is_accent) {
                ssize++;
                is_accent = false;
            } else if (int(s[i]) < 0) {
                is_accent = true;
            } else if (s[i] == '\033') {
                is_esc = true;
            } else {
                ssize++;
            }
        }
        //     cout << int(t[i]) << endl;
    }
    return ssize;
}
bool path_need_to_be_indented(filesystem::path path) {
    if (path_is_dir(path)) {
        return false;
    } else {
        if (path.has_parent_path()) {
            if (path.parent_path().has_parent_path()) {
                if (path.parent_path().parent_path().has_parent_path()) {
                    if (path.parent_path().parent_path().parent_path().filename().string() == "notes") {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
bool key_in_array(filesystem::path key, vector<filesystem::path> array) {
    for (filesystem::path key_array: array) {
        if (key_array == key) {
            return true;
        }
    }
    return false;
}
int find_in_array(filesystem::path key, vector<filesystem::path> array) {
    int i = 0;
    for (filesystem::path key_array: array) {
        if (key_array == key) {
            return i;
        }
        i++;
    }
    return -1;
}
string next_elt_of_map(map <string, filesystem::path> list_cours, string current_cours) {
    auto it = list_cours.find(current_cours);
    if (it != list_cours.end()) {
        it++;
        if (it != list_cours.end()) {
            return it->first;
        }
    }
    return "";
}
string previous_elt_of_map(map <string, filesystem::path> list_cours, string current_cours) {
    auto it = list_cours.find(current_cours);
    if (it != list_cours.end()) {
        it--;
        if (it != list_cours.end()) {
            return it->first;
        }
    }
    return "";
}
string next_elt_of_vector(vector <filesystem::path> list_matieres, string current_matiere) {
    auto it = list_matieres.begin();
    for (; it != list_matieres.end(); it++) {
        if (it->string() == current_matiere) {
            it++;
            if (it != list_matieres.end()) {
                return it->string();
            } else {
                return "";
            }
        }
    }
    return "";
}
string previous_elt_of_vector(vector <filesystem::path> list_matieres, string current_matiere) { // TOFIX
    auto it = list_matieres.begin();
    for (; it != list_matieres.end(); it++) {
        if (it->string() == current_matiere) {
            if (it != list_matieres.begin()) {
                it--;
                return it->string();
            } else {
                return "";
            }
            if (it != list_matieres.end()) {
                return it->string();
            }
        }
    }
    return "";
}
void draw_box(int x, int y, int w, int h){
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    cout << "\033[" << y << ";" << x << "H\u250f" << borderutf8 * (w-2) << "\u2513" << endl;
    for (int i = 1; i < h - 1; i++) {
        cout << "\033[" << y + i << ";" << x << "H\u2503" << space * (w-2) << "\u2503" << endl;
    }
    cout << "\033[" << y+h-1 << ";" << x << "H\u2517" << borderutf8 * (w-2) << "\u251b" << endl;
    return;
}


#endif //CPP_USEFULL_FUNCTION_H
