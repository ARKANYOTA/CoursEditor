//
// Created by ay on 2/2/22.
//
#include "include/conio.h"

#include <iostream>
#include <algorithm>
#include <json/json.h>
#include <fstream>

char config_path[] = "/home/ay/.config/cours2022.json";

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

struct notification{
    string message;
    int criticality;
    time_t emit_time;
    time_t death_time;
};
vector<notification> notifications;

int main() {
    /*
    ifstream ifs("/home/ay/.config/cours2022.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj); // reader can also read strings
    cout << "Book: " << obj["cours_path"].asString() << endl;
    cout << "Year: " << obj["editor"].asString() << endl;
    const Json::Value& characters = obj["characters"]; // array of characters
    for (int i = 0; i < characters.size(); i++){
        cout << "    name: " << characters[i]["name"].asString();
        cout << " chapter: " << characters[i]["chapter"].asUInt();
        cout << endl;
    }*/
    /*
    char down[4] = "\033[B";
    // char key[4];
    while(1){
        // get_kk(key, 0);
        string key = get_key_(0);
        cout << key << endl;

        cout << print_with_escape_char(key.c_str()) << endl;
        if (key[0] == 'n') {
            cout << "hello" << endl;
        }
        if(key[0] == 'q'){
            return 0;
        }
        if(key.substr(0, 3) == "\033[B"){
                    cout << "hello2" << endl;
        }
    }*/
    /*
    notifications.push_back({
            "hello",
            1,
            time(NULL),
            time(NULL) + 10
    });
    notifications.push_back({
                                    "hello2",
                                    1,
                                    time(NULL),
                                    time(NULL) + 10
                            });

    cout << notifications[0].message << endl;
    for(int i = 0; i < notifications.size(); i++){
        cout << notifications[i].message << endl;
    }
    */
    string nkey = get_key_(0);
    char k = nkey[0];
    int k2 = k;
    cout <<"xxx"<< print_with_escape_char(nkey) << "xxx|" << nkey << "|" << k << "|" << k2<<endl;



    // cout << string_cut("123ç56789a\033[1;1Hbcdéf", 8, 13) << endl;
    return 0;
}
