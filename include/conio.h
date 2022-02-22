// found on http://stackoverflow.com/a/7469410/1579481
#include <termios.h>
#include <stdio.h>
#include <string>
static struct termios old, newp;

std::string print_with_escape_char(std::string text){
    std::string to_return = "";
    for(int i = 0; i < text.length(); i++){
        if(text[i] == '\n'){
            to_return += "\\n";
        }
        else if(text[i] == '\t'){
            to_return += "\\t";
        }
        else if(text[i] == '\b'){
            to_return += "\\b";
        }
        else if(text[i] == '\x1b'){
            to_return += "\\x1b";
        }
        else if(text[i] == '\xce'){
            to_return += "\\xce";
        }
        else if(text[i] == '\x7f'){
            to_return += "\\x7f";
        }
        else{
            to_return += text[i];
        }
    }
    return to_return;
}



/* Initialize new terminal i/o settings */
void initTermios(int echo){
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  newp = old; /* make new settings same as old settings */
  newp.c_lflag &= ~ICANON; /* disable buffered i/o */
  newp.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &newp); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void){
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo){
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
std::string get_key_(int echo){
    initTermios(echo);
    char c1;
    char c2;
    char c3;
    char c4;
    char cccc[4];
    c1 = (char) getchar();
    cccc[0] = c1;
    if(c1 == '\x1b' || c1=='\xce'){
        c2 = (char) getchar();
        cccc[1] = c2;
        if(c2 == '\x5b' || c2=='O'){
            c3 = (char) getchar();
            cccc[2] = c3;
            if(c3 == '\x33' || c3=='1' || c3=='2'){
                c4 = (char) getchar();
                cccc[3] = c4;
            }
        }
    }
    resetTermios();
    return cccc;
}

char* get_kk(char* key, int echo) {
    initTermios(echo);
    char c1;
    char c2;
    char c3;
    char c4;
    c1 = (char) getchar();
    key[0] = c1;
    if(c1 == '\x1b' || c1=='\xce'){
        c2 = (char) getchar();
        key[1] = c2;
        if(c2 == '\x5b' || c2=='O'){
            c3 = (char) getchar();
            key[2] = c3;
            if(c3 == '\x33' || c3=='1' || c3=='2'){
                c4 = (char) getchar();
                key[3] = c4;
            }
        }
    }
    resetTermios();
    return key;
}

/* Read 1 character without echo */
char getch(void){
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void){
  return getch_(1);
}

char get_key(){
    char ch[2];
    ch[0] = getch();
    if (*ch == 27){
        ch[1] = getch();
        return *ch;
    }
    else{
        ch[1] = 0;
        return *ch;
    }
}



