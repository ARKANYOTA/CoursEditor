//
// Created by ay on 14/11/2021.
//

#ifndef CPP_CONSTANTES_H
#define CPP_CONSTANTES_H

// Faudrait faire ça en vector ou unordored_map
const string kKeyWidth[100] = {"TAB", "CAPS", "LFSH", "LCTL", "LWIN", "LALT", "SPCE", "BKSP","RTRN", "RTSH", "ESC", "DELE"};
const int    vKeyWidth[100] = {10    , 12    , 8     , 8     , 7     , 8     , 36,     10, 10, 14, 8, 8};
/*const string keysName[LINES][KEY_BY_LINE] = {
        {"ESC", "FK01", "FK02", "FK03", "FK04", "FK05", "FK06", "FK07", "FK08", "FK9", "FK10", "FK11", "FK12",           "PRSC", "SCLK", "PAUS"},
        {"TLDE", "AE01", "AE02", "AE03", "AE04", "AE05", "AE06", "AE07", "AE08", "AE09", "AE10", "AE11", "AE12", "BKSP", "INS", "HOME", "PGUP"},
        {"TAB", "AD01", "AD02", "AD03", "AD04", "AD05", "AD06", "AD07", "AD08", "AD09", "AD10", "AD11", "AD12", "BKSL",  "DELE", "END", "PHDN"},
        {"CAPS", "AC01", "AC02", "AC03", "AC04", "AC05", "AC06", "AC07", "AC08", "AC09", "AC10", "AC11", "RTRN"},
        {"LFSH", "AB01", "AB02", "AB03", "AB04", "AB05", "AB06", "AB07", "AB08", "AB09", "AB10", "RTSH"},
        {"LCTL", "LWIN", "LALT",                               "SPCE" , "RALT", "RWIN", "MENU", "RCTL", "LEFT", "DOWN", "UP", "RGHT"}
};
const int keysPos[LINES][KEY_BY_LINE] = { // keycode // don't have keycode 0
        {9     , 67    ,  68   ,    69 ,   70  ,    71 ,    72 , 73    , 74    , 75   , 76    , 95    , 96    ,           107   , 78    , 127   },
        {49    , 10    , 11    , 12    , 13    , 14    , 15    , 16   , 17    , 18  , 19    , 20      , 21    , 22      , 118  , 110   , 112   },
        {23    , 24    , 25    , 26    , 27    , 28    , 29    , 30    , 31    , 32    , 33    , 34    , 35    , 51    ,  119   , 115  , 117   },
        {66    , 38    , 39    , 40    , 41    , 42    , 43    , 44    , 45    , 46    , 47    , 48    , 36    },
        {50    , 52    , 53    , 54    , 55    , 56    , 57    , 58    , 59    , 60    , 61    , 62    },
        {37    , 133   , 64    ,                               65     , 108   , 94    , 135   , 99    , 113   , 116   , 111 , 114   }
};
const int keysPosMaxKeyByLine[LINES] = {15,16,16, 12, 11, 11};
*/
const string keysName[LINES][KEY_BY_LINE] = {
        {"ESC" , "FK01", "FK02", "FK03", "FK04", "FK05", "FK06", "FK07", "FK08", "FK09", "FK10", "FK11", "FK12", "DELE"},
        {"TLDE", "AE01", "AE02", "AE03", "AE04", "AE05", "AE06", "AE07", "AE08", "AE09", "AE10", "AE11", "AE12", "BKSP"},
        {"TAB" , "AD01", "AD02", "AD03", "AD04", "AD05", "AD06", "AD07", "AD08", "AD09", "AD10", "AD11", "AD12", "BKSL"},
        {"CAPS", "AC01", "AC02", "AC03", "AC04", "AC05", "AC06", "AC07", "AC08", "AC09", "AC10", "AC11", "RTRN"},
        {"LFSH", "LSGT","AB01", "AB02", "AB03", "AB04", "AB05", "AB06", "AB07", "AB08", "AB09", "AB10", "RTSH"},
        {"LCTL", "LWIN", "LALT",                               "SPCE" , "RALT", "RCTL", "LEFT", "DOWN", "UP", "RGHT"}
};

// il y a une range de 8-255 touches : plus d'info avec xmodmap -pke
// J'en ai que ~75 touches sur mon clavier (pas de pavé numérique)
const int keysPos[LINES][KEY_BY_LINE] = { // keycode // don't have keycode 0
        {9     , 67    , 68    , 69    , 70    , 71    , 72    , 73    , 74    , 75    , 76    , 95    , 96    , 119},
        {49    , 10    , 11    , 12    , 13    , 14    , 15    , 16    , 17    , 18    , 19    , 20    , 21    , 22},
        {23    , 24    , 25    , 26    , 27    , 28    , 29    , 30    , 31    , 32    , 33    , 34    , 35    , 51},
        {66    , 38    , 39    , 40    , 41    , 42    , 43    , 44    , 45    , 46    , 47    , 48    , 36    },
        {50    , 94    , 52    , 53    , 54    , 55    , 56    , 57    , 58    , 59    , 60    , 61    , 62    },
        {37    , 133   , 64    , 65                                    , 108   , 105   , 113   , 116   , 111 , 114   }
};
const int keysPosMaxKeyByLine[LINES] = {13,13,13, 12, 12, 9};
const string ControlKeys[100] = {"ISO_Level3_Shift", "Shift_L", "Shift_R"};
const string AltsGrs[4] = {"K: ", "S: ", "G: ", "G+S: "};

// TOFINISH
std::unordered_map<std::string, std::string> u = {
        {"AE",               "Æ"},
        {"Alt_L",            "LA"},
        {"Alt_R",            "RA"},
        {"BackSpace",        "⌫"},
        {"Control_L",        "LC"},
        {"Control_R",        "RC"},
        {"Delete",           "\u2421"},
        {"Down",             "\u2B07"},
        {"Escape",           "\u241B"},
        {"EuroSign",         "€"},
        {"F1",               "\u2170"},
        {"F10",              "\u2179"},
        {"F11",              "\u217a"},
        {"F12",              "\u217b"},
        {"F2",               "\u2171"},
        {"F3",               "\u2172"},
        {"F4",               "\u2173"},
        {"F5",               "\u2174"},
        {"F6",               "\u2175"},
        {"F7",               "\u2176"},
        {"F8",               "\u2177"},
        {"F9",               "\u2178"},
        {"ISO_Level3_Shift", "\u2327"},
        {"Left",             "\u2B05"},
        {"Caps_Lock",        "⇬"},
        {"Shift_L",          "L⇧"},
        {"Shift_R",          "R⇧"},
        {"OE",               "Œ"},
        {"Return",           "\u23CE"},
        {"Right",            "\u27A1"},
        {"Super_L",          "\u2318"},
        {"THORN",            "Þ"},
        {"Tab",              "↹"},
        {"Tslash",           "Ŧ"},
        {"Up",               "\u2B06"},
        {"ae",               "æ"},
        {"agrave",           "à"},
        {"ampersand",        "&"},
        {"apostrophe",       "'"},
        {"asciicircum",      "^"},
        {"asciitilde",       "~"},
        {"at",               "@"},
        {"backslash",        "\\"},
        {"bar",              "|"},
        {"braceleft",        "("},
        {"braceright",       ")"},
        {"bracketleft",      "["},
        {"bracketright",     "]"},
        {"ccedilla",         "ç"},
        {"cent",             "¢"},
        {"dead",             " "},
        {"dead",             "˛"},
        {"dead_abovedot",    "˙"},
        {"dead_abovering",   "ˍ"},
        {"dead_acute",       "´"},
        {"dead_breve",       "˘"},
        {"dead_caron",       "ˇ"},
        {"dead_cedilla",     "ç"},
        {"dead_circumflex",  "^"},
        {"dead_diaeresis",   "¨"},
        {"dead_doubleacute", "˝"},
        {"dead_grave",       "`"},
        {"dead_macron",      "¯"},
        {"dead_ogonek",      "˛"},
        {"dead_tilde",       "~"},
        {"degree",           "°"},
        {"dollar",           "$"},
        {"downarrow",        "↓"},
        {"eacute",           "é"},
        {"egrave",           "è"},
        {"equal",            "="},
        {"exclamdown",       "¡"},
        {"exclam",       "!"},
        {"asterisk",     "*"},
        {"colon",        ":"},
        {"semicolon",     ";"},
        {"slash",         "/"},
        {"question",      "?"},
        {"period",        "."},
        {"greater",       ">"},
        {"comma",          ","},
        {"fiveeighths",      "⅝"},
        {"grave",            "`"},
        {"guillemotleft",    "«"},
        {"idotless",         "ı"},
        {"leftarrow",        "←"},
        {"less",             "<"},
        {"minus",            "−"},
        {"notsign",          "¬"},
        {"numbersign",       "#"},
        {"oe",               "œ"},
        {"oneeighth",        "⅛"},
        {"onesuperior",      "¹"},
        {"oslash",           "ø"},
        {"paragraph",        "¶"},
        {"parenleft",        "("},
        {"parenright",       ")"},
        {"percent",          "%"},
        {"plus",             "+"},
        {"plusminus",        "±"},
        {"questiondown",     "¿"},
        {"quotedbl",         "\""},
        {"registered",       "®"},
        {"rightarrow",       "→"},
        {"seveneighths",     "⅞"},
        {"space",            "\u2420"},
        {"sterling",         "£"},
        {"threeeighths",     "⅜"},
        {"trademark",        "™"},
        {"tslash",           "ŧ"},
        {"twosuperior",      "²"},
        {"underscore",       "_"},
        {"uparrow",          "↑"},
        {"yen",              "¥"}
};

#endif