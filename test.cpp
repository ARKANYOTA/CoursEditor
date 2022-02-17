//
// Created by ay on 2/2/22.
//

#include <iostream>
#include <algorithm>

using namespace std;

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

string string_cut(string s, int start, int end = -1) {  // TOFIX
    string new_string = "";
    if (end == -1) {
        end = string_length(s);
    }
    int ssize = 0;
    bool is_accent = false;
    bool is_esc = false;
    for (int i = 0; i < s.size(); i++) {
        if (is_esc) {
            if (start <= ssize and ssize < end) {
                new_string += s[i];
            }
            if (s[i] == 'm' or s[i] == 'H') {
                is_esc = false;
                continue;
            }
        } else {
            if (int(s[i]) < 0 and is_accent) {
                ssize++;
                is_accent = false;
            } else if (not is_accent) {
                is_accent = true;
            } else if (s[i] == '\x1b') {
                is_esc = true;
                if (start <= ssize and ssize < end) {
                    new_string += s[i];
                }

            } else {
                ssize++;
            }
            if (start <= ssize and ssize < end) {
                new_string += s[i];
            }
        }
    }
    return new_string;
};

int main() {
    cout << string_cut("123ç56789a\033[1;1Hbcdéf", 8, 13) << endl;
    return 0;
}