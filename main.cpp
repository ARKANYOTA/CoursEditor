#include "include/conio.h"
#include "include/usefull_function.h"

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


// Entrain de dev ligne :590

//TODO
// - Tout le help
// - Cut avec la fonction personnalisée pour éviter les débordements dans l'écran
// - I18n

// -pouvoir changer le cours_path
//Bugs:
// - Metre un nombre maximale de notiications
// - Quand le dossier est suprimer, il met le fichier a été supprimer, et n'update pas les matieres
// Potentiel Bug:
// - Ferifier si le fichier config n'exsiste pas ou plus

//Optimisation
// - replace all border dans une liste d'élément.
// - Ne pas supprimer un fichier ou un dossier mais le metrre dans une corbeille
// - Faire un logger de toutes les actions
// - Bouger le windows size
// - Mettre des notification avec nom des actions et fichier modifier
// - Supprimer les debugvar
// - Faire le makefile / auto install config
// - Ajouter un :vim setf=...: sur chaque creation de fichier
// - Refaire touts les box/ size box/ position de chaque elmnet
// - Mettre les variables dans des classes/typedef/struct
// - Reorganiser le dossier Cours2022Git
// - Refaire le get_input
// - mettre le hiddenFiles dans le fichier notes
// - Faire un packet aur
// - Refaire le readme avec des images

//Help
// - n: Changer d'éditeur
// - i: Mode Insert
// - r: Mode Replace
// - ESC: Mode Normal
// - c: Crée une matière
// - m: Crée un cours
// - d: Supprime un cours ou matière
// - f: Fold un cours ou matière
// - Arrow: Navigue dans les cours ou matière
// - Enter: Ouvre un cours ou matière
// - h: Affiche les infos des cours ou matière
// - q: Quitte le programme
// - ?: Affiche les commandes
// - t: Affiche le 'table of content'
// - v: lis le cours dans le mode lecture quitable avec esc
// - e: Export all
// - R: Reload all files

using namespace std;

int current_mode = 0;
string modes[3] = {"NORMAL", "INSERT", "REPLACE"};
string path = "Hello/World/Markdown.md";
int current_editor = 0;
string editors[4] = {"vim", "nvim", "emacs", "notable"};
map <string, string> other_editors = {
        {".png",     "feh"},
        {".jpg",     "feh"},
        {".jpeg",    "feh"},
        {".gif",     "feh"},
        {".bmp",     "feh"},
        {".mp3",     "mpv"},
        {".mp4",     "mpv"},
        {".mkv",     "mpv"},
        {".webm",    "mpv"},
        {".pdf",     "zathura"},
        {".asprite", "asprite"},
        {".xcf",     "gimp"},
        {".html",    "brave"},
        {".odt",     "libreoffice"},
};
string cours_path = "/home/ay/Cours2022Git/notes";
string current_locale = "fr";
int cours_version = 0;

map <string, filesystem::path> list_cours;
string current_cours;
bool is_cours_selected = true;
bool is_visualisation = false;
bool is_toc = false;
bool is_infos = false;
// map<string, filesystem::path> list_matieres;
string current_matiere;
vector <filesystem::path> list_matieres;
// char config_path[] = "/home/ay/.config/cours2022.conf";
char config_path[] = "/home/ay/.config/cours2022.json";
char logger_path[] = "/home/ay/.config/cours2022.log";
vector <pair<string, string>> numbers;
vector <filesystem::path> list_fold;
bool debug_mode = false;
vector <pair<string, string>> path_info;
struct notification {
    string message;
    int criticality;
    time_t emit_time;
    time_t death_time;
    time_t duration;
};
vector <notification> notifications;
vector <pair<string, pair < std::time_t, std::time_t>>>
list_notifications;
string debugvar = "debug:";
Json::Value number;

// --------------------------------------------------
// Notifications
// --------------------------------------------------
void create_notification(string message, time_t t) {
    // time = when you stop the notification
    // list_notifications.push_back(make_pair(message, make_pair(time(0), time(0) + t)));
    notifications.push_back({message, 0, time(0), time(0) + t, t});
    ofstream logger(logger_path, ios::app);
    logger << "Notification: " << message << " " << t << endl;
    logger.close();
}

void check_notifications() {
    for (int i = 0; i < notifications.size(); i++) {
        if (notifications[i].death_time <= time(0)) {
            notifications.erase(notifications.begin() + i);
        }
    }
}

void draw_notifications() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    int i = size.ws_row - 2;
    for (notification notif: notifications) {
        int time_pass = time(0) - notif.emit_time;
        int longueur = size.ws_col / 2 - 9;
        int max_bar_longueur = longueur - 2;
        int bar_longueur = max_bar_longueur * time_pass / notif.duration;
        int text_longueur = notif.message.size();

        draw_box(4, i - 2, longueur, 3);
        if (bar_longueur < text_longueur) {
            cout << "\033[" << i - 1 << ";" << 6 << "H"
                 << "\033[107;30m" << notif.message.substr(0, bar_longueur)
                 << "\033[0m" << notif.message.substr(bar_longueur) << "\033[0m";
        } else {
            cout << "\033[" << i - 1 << ";" << 6 << "H\033[107m" << string(bar_longueur, ' ');
            cout << "\033[107;30m\033[" << i - 1 << ";" << 6 << "H" << notif.message << "\033[0m";
        }
        i = i - 3;
    }
}
// --------------------------------------------------
// End Notifications
// --------------------------------------------------

int send_confirmation(string message, string valid_message, string invalid_message, char valid_key = 'o',
                      bool has_invalid_key = false, char invalid_key = 'n') {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    int center_col = (size.ws_col / 2) - (string_length(message) / 2);
    // for (int i = 0; i < 5; i++) {
    //     cout << "\033[" << (size.ws_row) / 2 - 1 + i << ";" << (center_col - 3) << "H\u2503" << string(" ") * (message.length() + 2) << "\u2503" << endl;
    // }
    // cout << "\033[" << (size.ws_row) / 2 - 2 << ";" << (center_col - 3) << "H\u250f" << borderutf8 * (message.length() + 2) << "\u2513" << endl;
    // cout << "\033[" << (size.ws_row) / 2 << ";" << (center_col - 1) << "H" << message << endl;
    // cout << "\033[" << (size.ws_row) / 2 + 2 << ";" << (center_col + 1) << "H" << valid_message << endl;
    // cout << "\033[" << (size.ws_row) / 2 + 2 << ";" << (center_col + string_length(message) - invalid_message.length() - 4) << "H" << invalid_message << endl;
    // cout << "\033[" << (size.ws_row) / 2 + 4 << ";" << (center_col - 3) << "H\u2517" << borderutf8 * (message.length() + 2) << "\u251b" << endl;

    draw_box(center_col - 3, size.ws_row / 2 - 1, message.length() + 6, 5);
    cout << "\033[" << (size.ws_row) / 2 << ";" << (center_col - 1) << "H" << message << endl;
    cout << "\033[" << (size.ws_row) / 2 + 2 << ";" << (center_col + 1) << "H" << valid_message << endl;
    cout << "\033[" << (size.ws_row) / 2 + 2 << ";"
         << (center_col + string_length(message) - invalid_message.length() - 4) << "H" << invalid_message << endl;

    char nkey = getch_(0);
    if (nkey == valid_key) {
        return true;
    }
    if (has_invalid_key and nkey == invalid_key) {
        return 2;
    }
    return false;
}

// --------------------------------------------------
// Generation, et loading, Reload
// --------------------------------------------------
void load_config() {
    ifstream ifs("/home/ay/.config/cours2022.json");
    Json::Reader reader;
    Json::Value obj;
    reader.parse(ifs, obj); // reader can also read strings
    cours_path = obj["cours_path"].asString();
    cours_version = obj["cours_version"].asInt();
    current_locale = obj["current_locale"].asString();
    // Dans fold
    for (Json::Value::ArrayIndex i = 0; i != obj["fold"].size(); i++) {
        // list_fold.push_back(obj["fold"][i]);//.toStyledString());
        filesystem::path p = obj["fold"][i].asString();
        list_fold.push_back(p); //.toStyledString());
        //if (obj["fold"][i].isMember("attr1")) {
        //    values.push_back(obj["fold"][i]["attr1"].asString());
        //}
    }
}

void save_config(){
    Json::Value obj;
    obj["cours_path"] = cours_path;
    obj["cours_version"] = cours_version;
    for (filesystem::path p: list_fold) {
        obj["fold"].append(p.string());
    }
    ofstream ofs("/home/ay/.config/cours2022.json");
    ofs << obj;
    ofs.close();
}

void generate_path_info() {
    path_info.clear();
    if (!path_is_dir(path)) {
        filesystem::path current_path = path;
        path_info.push_back(pair<string, string>("name", current_path.filename().string()));
        path_info.push_back(pair<string, string>("extention", current_path.extension().string()));

        if (filesystem::exists(current_path) and not path_is_dir(current_path)) {
            FILE *file = fopen(current_path.string().c_str(), "r");
            // count word lines and char
            int word_count = 0;
            int char_count = 0;
            int line_count = 0;
            char c;
            while ((c = fgetc(file)) != EOF) {
                if (c == '\n') {
                    line_count++;
                } else {
                    char_count++;
                    if (c == ' ') {
                        word_count++;
                    }
                }
            }
            fclose(file);
            path_info.push_back(pair<string, string>("line", to_string(line_count)));
            path_info.push_back(pair<string, string>("word", to_string(word_count)));
            path_info.push_back(pair<string, string>("char", to_string(char_count)));
            if (other_editors.find(current_path.extension().string()) != other_editors.end()) {
                path_info.push_back(pair<string, string>("execwith", other_editors[current_path.extension().string()]));
            } else {
                path_info.push_back(pair<string, string>("execwith", "currenteditor"));
            }


        }
    }
}

// // Reload
void reload_matieres() {
    list_matieres.clear();
    if (current_cours.substr(0, 2) == "+ ") {
        for (const auto &file: filesystem::directory_iterator(list_cours[current_cours].string())) {
            // File name
            list_matieres.push_back(file.path());
        }
    } else {
        list_matieres.push_back(list_cours[current_cours]);
    }
    sort(list_matieres.begin(), list_matieres.end());
    current_matiere = list_matieres.begin()->string();
    //loop inside the vector
    vector <filesystem::path> list_matieres2;

    //Copy the vector to another vector
    for (std::vector<filesystem::path>::iterator it = std::begin(list_matieres); it != std::end(list_matieres); ++it) {
        if (!key_in_array(*it, list_fold)) {
            list_matieres2.push_back(*it);
        } else {
            // Pas besoin de faire les fichiers si il est dans le fold
            list_matieres2.push_back("Non");
        }
    }
    int i = 0;
    set <filesystem::path> set_matieres;

    for (std::vector<filesystem::path>::iterator it = std::begin(list_matieres2);
         it != std::end(list_matieres2); ++it) {
        i++;
        if (*it != "Non") {
            if (path_is_dir(it->string())) {
                set_matieres.clear();
                for (auto &entry: filesystem::directory_iterator(it->string())) {
                    set_matieres.insert(entry.path());
                }

                for (const auto &file: set_matieres) {
                    // File name
                    list_matieres.insert(begin(list_matieres) + i, file);
                    i++;
                }
            }
        }
    }
    return;
}

void reload_cours() {
    list_cours.clear();
    for (const auto &file: filesystem::directory_iterator(cours_path)) {
        // File name

        list_cours.insert(
                pair<string, filesystem::path>((file.is_directory() ? "+ " : "- ") + file.path().filename().string(),
                                               file.path()));
    }
    current_cours = list_cours.begin()->first;
    reload_matieres();
}

// --------------------------------------------------
// Draw
// --------------------------------------------------
void draw_cours() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    cout << (is_cours_selected ? "\033[33;1;100m" : "") << "\033[2;2H Cours: \033[0m" << endl;
    int i = 0;
    for (auto const&[key, val]: list_cours) {
        cout << "\033[" << i + 4 << ";4H" << (key == current_cours ? "\033[33m" : "")
             << key.substr(0, size.ws_col / 2 - 4) << "\033[0m" << endl;  //.substr(1)
        i++;
    }
}

void draw_matiere() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    cout << (!is_cours_selected ? "\033[33;1;100m" : "") << "\033[2;" << size.ws_col / 2 + 2 << "H Matiere "
         << current_cours.substr(2) << ": \033[0m" << endl;
    int i = 0;
    int j = 0;
    for (std::vector<filesystem::path>::iterator it = std::begin(list_matieres); it != std::end(list_matieres); ++it) {
        cout << "\033[" << i + 4 << ";" << size.ws_col / 2 + 4 << "H"
             << (*it == current_matiere ? "\033[33m" : "")
             // Si il est dans le fold: ">" sinon (Si c'est un dossier "+" sinnon "-")
             << (key_in_array(*it, list_fold) ? "> " : ((path_is_dir(*it)) ? "+ " : "- "))
             << ((path_need_to_be_indented(*it)) ? "    " : "")
             << ((*it).filename().string()).substr(0, (size.ws_col / 2) - 8) << "\033[0m" << endl;  //.substr(1)
        // Check la size a print pour éviter les débordements
        i++;
        if (*it == current_matiere) {
            if (is_infos) {
                j = i;
            }
        }


        //generate_path_info();
        //cout <<  "\033[" << i + 4 << ";" << size.ws_col / 2 + 4 << "H" << "ls" << "\033[0m" << endl;
    }
    // for  i in pathinfo
    if (is_infos && !path_is_dir(path)) {
        generate_path_info();
        int max_len_val = 0;
        for (auto const&[key, val]: path_info) {
            if (key.length() + val.length() + 3 > max_len_val) {
                max_len_val = key.length() + val.length() + 3;
            }
        }

        draw_box(size.ws_col / 2 + 8, j + 4, max_len_val + 4, path_info.size() + 2);
        for (auto const&[key, val]: path_info) {
            cout << "\033[" << j + 5 << ";" << size.ws_col / 2 + 10 << "H" << key << ": " << val << "\033[0m" << endl;
            j++;
        }
    }

}

void draw_cours_content() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    cout << (!is_cours_selected ? "\033[33;1;100m" : "") << "\033[2;" << size.ws_col / 2 + 2 << "H Contenu de "
         << current_cours.substr(2) << ": \033[0m" << endl;
    // Lire le fichier cours
    ifstream myfile;
    myfile.open(list_cours[current_cours].string());
    int i = 0;
    if (myfile.is_open()) {
        while (myfile) {
            string line;
            getline(myfile, line);
            cout << "\033[" << 4 + i << ";" << size.ws_col / 2 + 3 << "H" << line.substr(0, size.ws_col / 2 - 2)
                 << endl;
            i++;
        }
    }
}

void draw_interface() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    clear_screen();
    //"<Mode> <Path>       <éditeur>"
    string current_mode_name = modes[current_mode];
    string current_editor_name = editors[current_editor];
    string bottom_text(size.ws_col - current_mode_name.length() - 2 - current_editor_name.length() - 2, ' ');
    cout << "\033[42;30;1m\033[" << size.ws_row - 1 << ";" << "0H " << current_mode_name << " \033[0m";
    for (int i = 0; i < path.length(); i++) {
        bottom_text[i + 1] = path[i];
    }
    cout << "\033[100;34m\033[" << size.ws_row - 1 << ";" << current_mode_name.length() + 3 << "H" << bottom_text
         << "\033[0m";
    cout << "\033[44;32;1m\033[" << size.ws_row - 1 << ";" << current_mode_name.length() + 3 + bottom_text.length()
         << "H " << current_editor_name << " \033[0m";
    int color;
    for (int i = 0; i < size.ws_row - 2; i++) {
        if ((is_cours_selected and i < (size.ws_row - 2)/2) or (!is_cours_selected and i >= (size.ws_row - 2)/2)) {
            color = 31; // Rouge
        } else {
            color = 30; // Noir
        }
        // bar centrale
        cout << "\033[107;" << color << "m\033[" << i + 1 << ";" << size.ws_col / 2 << "H" << "\u2503" << "\033[0m";
    }
    draw_cours();
    if (current_cours[0] == '+') {
        draw_matiere();
    } else {
        draw_cours_content();
    }
    // Draw notification
    check_notifications();
    draw_notifications();
}

void draw_debug() {
    clear_screen();
    cout << "courspath:" << cours_path << endl;
    cout << "path:" << path << endl;
    for (auto &c: numbers) {
        cout << "    Number :" << c.first << "|" << c.second << endl;
    }
    for (auto &c: list_fold) {
        cout << "    ListFold :" << c << endl;
    }
    for (Json::Value::const_iterator it = number.begin(); it != number.end(); ++it)
        cout << it.key().asString() << endl;// << ':' << it->asInt() << '\n';


    /*
    cout << "current_mode = " << current_mode << endl;
    cout << "current_editor = " << current_editor << endl;
    cout << "current_cours = " << current_cours << endl;
    cout << "current_matiere = " << current_matiere << endl;
    cout << "path = " << path << endl;
    cout << "current_mode_name = " << modes[current_mode] << endl;
    cout << "current_editor_name = " << editors[current_editor] << endl;
    for (auto &c: list_cours) {
        cout << "    Cours : " << c.first << "|" << c.second << endl;
    }
    for (auto &m: list_matieres) {
        cout << "    ListMatiere : " << m.string() << endl;
    }
    // for i in vector
    for (auto &c: numbers) {
        cout << "    Number : " << c.first << "|" << c.second << endl;
    }
    // for in list_fold
    for (auto &c: list_fold) {
        cout << "    ListFold : " << c << endl;
    }
    for (auto &c: list_notifications) {
        cout << "    ListNotification : " << c.first << "|" << c.second.second << endl;
    }

    */
}

void draw_visualisation() {
    // lire les ligne du fichier courrant
    filesystem::path tmp_path = path;
    ifstream input(path);
    if (filesystem::is_directory(tmp_path)) {
        //SEND Notifpeut pas visualisier un dosier
        is_visualisation = false;
        return;
    }
    clear_screen();
    for (string line; getline(input, line);) {
        cout << line << endl;
    }
}

void draw_toc() {
    // lire les ligne du fichier courrant
    filesystem::path tmp_path = path;
    ifstream input(path);
    if (filesystem::is_directory(tmp_path)) {
        //SEND Notifpeut pas toc un dosier
        is_visualisation = false;
        return;
    }
    clear_screen();
    for (string line; getline(input, line);) {
        if (line[0] == '#') {
            cout << line << endl;
        }
    }
}
// --------------------------------------------------
// End of draw
// --------------------------------------------------

void reload() {  // A mettre de la de la lige :628
    string tmp_path = path;
    load_config();
    reload_cours();
    path = tmp_path;

}

void re_draw_all() {
    if (!debug_mode) {
        draw_interface();
    } else {
        draw_debug();
    }
    if (is_visualisation) {
        draw_visualisation();
    }
    if (is_toc) {
        draw_toc();
    }
}
// --------------------------------------------------
// Creation inttercalaire et cours
// --------------------------------------------------


string get_input_string(string message, string start_with = "> ") {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    re_draw_all();
    int center_col = (size.ws_col / 2) - 50;
    draw_box(center_col, size.ws_row / 2 - 3, 100, 5);

    cout << "\033[" << (size.ws_row) / 2 - 3 << ";" << (center_col) + 2 << "H┥" << message << "┝" << endl;
    cout << "\033[" << (size.ws_row) / 2 - 1 << ";" << (center_col) + 4 << "H"<< start_with;
    string input_string;
    cin >> input_string;
    getch_(0);
    return input_string;
}

void get_level(string message, bool how_many_level, int *level1, int *level2) {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    re_draw_all();
    bool is_first_level = true;
    int *selected_level = new int;

    int center_col = (size.ws_col / 2) - 50;
    draw_box(center_col, size.ws_row / 2 - 3, 100, 5);
    cout << "\033[" << (size.ws_row) / 2 - 3 << ";" << (center_col) + 2 << "H┥" << message << "┝" << endl;
    bool boucle_run = true;
    while(boucle_run){
        selected_level = (is_first_level ? level1 : level2);
        if(is_first_level) cout << "\033[31m";
        draw_box(center_col + 3, size.ws_row / 2-2, 4, 3);
        cout << "\033[0m";
        cout << "\033[" << (size.ws_row) / 2 - 1 << ";" << (center_col) + 4 << "H" << *level1 << endl;
        if (how_many_level) {
            if(!is_first_level) cout << "\033[31m";
            draw_box(center_col + 7, size.ws_row / 2-2, 4, 3);
            cout << "\033[0m";
            cout << "\033[" << (size.ws_row) / 2 - 1 << ";" << (center_col) + 8 << "H" << *level2 << endl;
        }
        string nkey = get_key_(0);
        if(nkey[0] == 'q' or nkey[0] == '\n') {
            boucle_run = false;
        }
        if(nkey == "\033[B") {
            if(*selected_level > 0) {
                *selected_level -= 1;
            }
        }
        if(nkey == "\033[A") {
            if(*selected_level < 99) {
                *selected_level += 1;
            }
        }

        if(how_many_level) {
            if(nkey == "\033[C" or nkey == "\033[D") {
                is_first_level = !is_first_level;
            }
        }
        if(48 <= (int)nkey[0] && (int)nkey[0] <= 57) {
            if (*selected_level == 0) {
                *selected_level = (int)nkey[0] - 48;
            }else if(*selected_level <= 9) {
                *selected_level = (*selected_level * 10) + (int)nkey[0] - 48;
            }
        }
        if(nkey == "\x7f") {
            *selected_level = *selected_level / 10;
        }
    }
    return;
}

void create_intercalary() {
    int level1 = 0;
    int level2 = 0;
    re_draw_all();
    cout << endl;
    get_level("Niveau de l'intercalaire: ", 0, &level1, &level2);
    // verify if path need to bee indented
    string input_string = get_input_string("Nom de l'intercalaire : ", to_string(level1) + ".");
    if (input_string.length() == 0) {
        create_notification("Le nom de l'intercalaire ne peut pas être vide", 5);
        return;
    }
    // string where_is_file = current_matiere + "/" + input_string + ".md";
    string where_is_file;
    if (path_need_to_be_indented(current_matiere)) {
        filesystem::path tmp_current_matiere_path(current_matiere);
        where_is_file = tmp_current_matiere_path.parent_path().parent_path().string() ;
    }else{
        filesystem::path tmp_current_matiere_path(current_matiere);
        where_is_file = tmp_current_matiere_path.parent_path().string();
    }
    where_is_file = where_is_file + "/" + to_string(level1) + "." + input_string;
    if (filesystem::exists(where_is_file)) {
        create_notification("Ce cours existe déjà", 5);
        return;
    }
    // crée un dossier
    if (filesystem::create_directory(where_is_file)) {
        create_notification("Le cours a été créé", 5);
    }

    reload_matieres();
    return;
}

//TODO Pouvoir mettre autre chose que des .md
void create_course() {
    re_draw_all();
    int what_beetween = send_confirmation("                  Ou voulez-vous le crée ?                   ", "A la racine(r)", "Dans le fichier Courant(c)",
                                          'r', true, 'c');
    if (what_beetween == 1) {
        // A la racine
        re_draw_all();
        string input_string = get_input_string("Nom du cours : ");
        if (input_string.length() == 0) {
            create_notification("Le nom du cours ne peut pas être vide", 5);
            return;
        }
        string where_is_file = cours_path + "/" + current_cours.substr(2) + "/" + input_string + ".md";
        if (filesystem::exists(where_is_file)) {
            create_notification("Ce cours existe déjà", 5);
            return;
        }
        string text = "# " + input_string + "\n";
        ofstream file(where_is_file);
        file << text;
        file.close();
        reload_matieres();
        return;
    }
    if (what_beetween == 2) {
        // Dans le fichier courant
        int level1 = 0;
        int level2 = 0;
        re_draw_all();
        cout << endl;
        get_level("Niveau du cours : ", 1, &level1, &level2);
        // verify if path need to bee indented
        string input_string = get_input_string("Nom du cours : ", to_string(level1) + "." + to_string(level2) + ".");
        if (input_string.length() == 0) {
            create_notification("Le nom du cours ne peut pas être vide", 5);
            return;
        }
        // string where_is_file = current_matiere + "/" + input_string + ".md";
        string where_is_file;
        if (path_need_to_be_indented(current_matiere)) {
            filesystem::path tmp_current_matiere_path(current_matiere);
            where_is_file = tmp_current_matiere_path.parent_path().string() ;
        }else{
            where_is_file = current_matiere;
        }
        where_is_file = where_is_file + "/" + to_string(level1) + "." + to_string(level2) + "." + input_string + ".md";
        if (filesystem::exists(where_is_file)) {
            create_notification("Ce cours existe déjà", 5);
            return;
        }
        string text = "# " + input_string + "\n";
        ofstream file(where_is_file);
        file << text;
        file.close();
        reload_matieres();
        return;
    }
    return;
}

void create_matiere(){
    re_draw_all();
    string input_string = get_input_string("Nom de la matière : ");
    if (input_string.length() == 0) {
        create_notification("Le nom de la matière ne peut pas être vide", 5);
        return;
    }
    string where_is_file = cours_path + "/" + input_string;
    if (filesystem::exists(where_is_file)) {
        create_notification("Cette matière existe déjà", 5);
        return;
    }
    if (filesystem::create_directory(where_is_file)) {
        create_notification("La matière a été créé", 5);
    }
    //TODO reload
    return;
}

void choose_what_create() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    string message = "             Que voulez-vous créer ?             ";
    string valid_message = "Une Intercalaire (i)";
    string invalid_message = "Un Cours (c)";
    int what_choose = send_confirmation(message, valid_message, invalid_message, 'i', true, 'c');
    if (what_choose == 1) {
        create_intercalary();
    } else if (what_choose == 2) {
        create_course();
    } else {
        create_notification("Vous avez annulé la création", 3);
    }
    return;


}

// --------------------------------------------------
// End of creation
// --------------------------------------------------
void execute_file() {
    if (!path_is_dir(current_matiere)) {
        filesystem::path current_path = path;
        // if extention is in other_editors keys
        if (other_editors.find(current_path.extension().string()) != other_editors.end()) {
            string command = other_editors[current_path.extension().string()];
            command.append(" \"");
            command.append(current_matiere);
            command.append("\"");
            system(command.c_str());

        } else {
            system((editors[current_editor] + " \"" + path +"\"").c_str());
        }

    } else {
        if (send_confirmation("Etez vous sur de vouloir lancer votre editeur sur ce dossier ?", "Oui lance (y/o)",
                              "Ne pas lancer")) {
            system((editors[current_editor] + " \"" + current_matiere + "\"").c_str());
        }
    }
}

void delete_current_path() {
    if (path_is_dir(path)) { // TODO
        if (send_confirmation("Voulez vous vraiment supprimer le dossier " + path + " ?", "Oui", "Non")) {
            filesystem::remove_all(path);
        }
    } else {
        if (send_confirmation("Voulez vous vraiment supprimer le éééééééééfichier " + path + " ?", "Oui", "Non")) {
            create_notification("Le fichier à été supprimé", 140);
            filesystem::remove(path);
            reload_matieres();
        }
    }
}

// --------------------------------------------------
// Git push
// --------------------------------------------------

void github_push() {

    filesystem::path tmp_course_path(cours_path);
    string command = "cd " + tmp_course_path.parent_path().string() + " && git add . && git commit -m \"update numéro "+ to_string(cours_version) +"\" && git push";
    int stderroe = system(command.c_str());
    if(stderroe == 0){
        create_notification("Votre cours a été mis à jour sur Github", 5);
        cours_version++;
        save_config();
    }else{
        create_notification("Erreur lors de la mise à jour sur Github", 5);
    }
    cout << "Press key to continue" << endl;
    getch_(0);
    cout << endl;
}

// --------------------------------------------------
// Main
// --------------------------------------------------
int main(void) {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    int a = 1000;
    //desactiver le cursseur
    cout << "\033[?25l";
    load_config();
    reload_cours();


    while (a--) {
        path = current_matiere;

        re_draw_all();
        path = current_matiere;
        // draw_box(5,5,80, 25);
        // cout << debugvar << endl;  // Je ne sais pour quelle raison mais si on print un string ou un endl, le send_confirmation ne fonctionne pas
        // cout << "";

        cout << "\033[" << size.ws_row - 1 << ";" << 3 << "H" << debugvar <<"|" << current_matiere << endl;
        string key = get_key_(0);
        if (!is_visualisation) {
            if (key[0] == 'n') current_editor = (current_editor + 1) % (editors->length() + 1);
            if (key[0] == 'i') current_mode = 1;
            if (key[0] == 'r') current_mode = 2;
            if (is_cours_selected) {
                if (key == "\033[B") {
                    if (next_elt_of_map(list_cours, current_cours) != "") {
                        current_cours = next_elt_of_map(list_cours, current_cours);
                    }
                    reload_matieres();
                }
                if (key == "\033[A") {
                    if (previous_elt_of_map(list_cours, current_cours) != "") {
                        current_cours = previous_elt_of_map(list_cours, current_cours);
                    }
                    reload_matieres();
                }
            } else {
                if (key == "\033[B") {
                    if (next_elt_of_vector(list_matieres, current_matiere) != "") {
                        current_matiere = next_elt_of_vector(list_matieres, current_matiere);
                    }
                }
                if (key == "\033[A") {
                    if (previous_elt_of_vector(list_matieres, current_matiere) != "") {
                        current_matiere = previous_elt_of_vector(list_matieres, current_matiere);
                    }
                }
            }
            if (key == "\033[D" or key == "\033[C") {
                is_cours_selected = !is_cours_selected;
            }
            if (key[0] == 'q') {
                //Reactiver le curseur
                cout << "\033[?25h";
                return 0;
            }
            if (key[0] == 'v') is_visualisation = !is_visualisation;

            if (key[0] == 't') is_toc = !is_toc;
            if (key == "\033\033") current_mode = 0;
            if (key[0] == '\n') execute_file();  // TODO: Execute en fonction de l'extension
            if (key[0] == 'h') is_infos = !is_infos;
            if (key[0] == 'R') reload_cours();
            if (key[0] == '.') {
                if (debug_mode) {
                    create_notification("Close debug menu", 14);
                    debug_mode = !debug_mode;
                } else {
                    if (send_confirmation("Do you want to enable debug mode ?", "Yes(y|o)", "Non TG(autre)")) {
                        debug_mode = true;
                        create_notification("Launch debug menu", 14);
                    } else {
                        debug_mode = false;
                    }
                }
            }
            if (key[0] == 'd') delete_current_path();
            if (key[0] == 'f') {  // TOFINISH TODO
                filesystem::path tmp_path = current_cours;

                if (key_in_array(path, list_fold)) {
                    if (key_in_array(path, list_fold)) {
                        list_fold.erase(list_fold.begin() + find_in_array(path, list_fold));
                        reload_matieres();
                        save_config();
                    }
                } else {
                    if (path_is_dir(path)) {
                        list_fold.push_back(path);
                        reload_matieres();
                        save_config();
                    } else {
                        create_notification("Impossible de le fold c'est pas un dossier", 9);
                    }
                }
            }
            if (key == "\x1b[15") {  // F5 for reaload
                reload();
            }
            if (key[0] == 'e') cout << "Export all" << endl;
            if (key[0] == 'c') {
                choose_what_create();
            };
            if (key[0] == 'm'){
                create_matiere();
                reload_cours();
            }
            if (key[0] == '?') cout << "Affiche les commandes (Ceci...)" << endl;
            if (key[0] == 'g'){
                // Github push
                if (send_confirmation("Do you want to push your changes to the github ?", "Yes(y|o)", "Non TG(autre)")) {
                    github_push();
                }
            };
        } else {
            if (key[0] == 'v') is_visualisation = !is_visualisation;
            if (key[0] == 't') is_toc = !is_toc;
        }
    }
    return 0;

}
