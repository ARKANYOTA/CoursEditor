//
// Created by ay on 14/11/2021.
//
#ifndef CPP_USEFULL_FUNCTION_H
#define CPP_USEFULL_FUNCTION_H


string convertToString(char *a, int size) {
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
    }
    return s;
}


#endif //CPP_USEFULL_FUNCTION_H
