//
// Created by ay on 2/21/22.
//
#include "conio.h"
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

#ifndef COURSEDITOR_DRAW_FUNCTIONS_H
#define COURSEDITOR_DRAW_FUNCTIONS_H

string get_input_string(string message){
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    reload();

    int center_col = (size.ws_col / 2) - (string_length(message) / 2);
    for (int i = 0; i < 5; i++) {
        cout << "\033[" << (size.ws_row) / 2 - 1 + i << ";" << (center_col - 3) << "H\u2503"
             << string(" ") * (message.length() + 2) << "\u2503" << endl;
    }
    cout << "\033[" << (size.ws_row) / 2 - 2 << ";" << (center_col - 3) << "H\u250f"
         << borderutf8 * (message.length() + 2) << "\u2513" << endl;
    cout << "\033[" << (size.ws_row) / 2 << ";" << (center_col - 1) << "H" << message << endl;
    // cout << "\033[" << (size.ws_row) / 2 + 2 << ";" << (center_col + 1) << "H" << valid_message << endl;
    // cout << "\033[" << (size.ws_row) / 2 + 2 << ";"
    //      << (center_col + string_length(message) - invalid_message.length() - 4) << "H" << invalid_message << endl;
    // cout << "\033[" << (size.ws_row) / 2 + 4 << ";" << (center_col - 3) << "H\u2517"
    //      << borderutf8 * (message.length() + 2) << "\u251b" << endl;
    getch_(0);
    return "mk";
}

#endif //COURSEDITOR_DRAW_FUNCTIONS_H
