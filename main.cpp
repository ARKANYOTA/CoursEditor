#include "include/conio.h"

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


//TODO
// - Tout le help
// - Crée  cours matières
// - Fold
// - Ajoute des cours
// - Ajoute des matières
// - Ajoute des sous matières
// - Notifications

//Bugs:
// -

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
map <string, filesystem::path> list_cours;
string current_cours;
bool is_cours_selected = true;
bool is_visualisation = false;
bool is_toc = false;
bool is_infos = false;
// map<string, filesystem::path> list_matieres;
string current_matiere;
vector <filesystem::path> list_matieres;
char config_path[] = "/home/ay/.config/cours2022.conf";
vector <pair<string, string>> numbers;
vector <filesystem::path> list_fold;
bool debug_mode = false;
vector <pair<string, string>> path_info;
vector <pair<string, pair < std::time_t, std::time_t>>>
list_notifications;
string debugvar = "debug:";
string borderutf8 = "\u2501";

string operator*(string a, unsigned int b) {
    string output = "";
    while (b--) {
        output += a;
    }
    return output;
}

void create_notification(string message, time_t t) {
    // time = when you stop the notification
    list_notifications.push_back(make_pair(message, make_pair(time(0), time(0) + t)));
}

void check_notifications() {
    //TODO
    for (int i = 0; i < list_notifications.size(); i++) {
        if (list_notifications[i].second.second < time(0)) {
            cout << list_notifications[i].first << endl;
            list_notifications.erase(list_notifications.begin() + i);
        }
    }
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

bool send_confirmation(string message, string valid_message, string invalid_message) {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    int center_col = (size.ws_col / 2) - (string_length(message) / 2);
    for (int i = 0; i < 5; i++) {
        cout << "\033[" << (size.ws_row) / 2 - 1 + i << ";" << (center_col - 3) << "H\u2503"
             << string(" ") * (message.length() + 2) << "\u2503" << endl;
    }
    cout << "\033[" << (size.ws_row) / 2 - 2 << ";" << (center_col - 3) << "H\u250f"
         << borderutf8 * (message.length() + 2) << "\u2513" << endl;
    cout << "\033[" << (size.ws_row) / 2 << ";" << (center_col - 1) << "H" << message << endl;
    cout << "\033[" << (size.ws_row) / 2 + 2 << ";" << (center_col + 1) << "H" << valid_message << endl;
    cout << "\033[" << (size.ws_row) / 2 + 2 << ";"
         << (center_col + string_length(message) - invalid_message.length() - 4) << "H" << invalid_message << endl;
    cout << "\033[" << (size.ws_row) / 2 + 4 << ";" << (center_col - 3) << "H\u2517"
         << borderutf8 * (message.length() + 2) << "\u251b" << endl;
    char nkey = getch_(0);  // TODO: Erreur si appuie sur une touche a echap (F3 ou autre)
    if (nkey == 'o' or nkey == 'y') {
        return true;
    }
    return false;
}

void load_config() {
    ifstream config_file(config_path);
    filesystem::path fold_path;
    if (config_file.is_open()) {
        string line;
        bool is_number = false;
        bool is_fold = false;
        while (getline(config_file, line)) {
            if (is_number) {
                if (line.find("- ") != string::npos) {
                    numbers.push_back(make_pair(line.substr(2, line.find(": ") - 2), line.substr(line.find(": ") + 2)));
                } else {
                    is_number = false;
                }
            }
            if (is_fold) {
                if (line.find("- ") != string::npos) {

                    list_fold.push_back(cours_path + "/" + line.substr(2));
                } else {
                    is_fold = false;
                }
            }
            if (line.find("cours_path") != string::npos) {
                cours_path = line.substr(line.find("=") + 2);
            }
            if (line.find("editor") != string::npos) {
                string editor = line.substr(line.find("=") + 2); //, line.length()-line.find("=")-2);
                for (int i = 0; i <= editors->length(); i++) {
                    if (editors[i] == editor) {
                        current_editor = i;
                    }
                }
            }
            if (line.find("number:") != string::npos) {
                is_number = true;
            }
            if (line.find("fold:") != string::npos) {
                is_fold = true;
            }

        }
        config_file.close();
    }
}


void clear_screen() {
    cout << "\033[2J\033[1;1H";
}


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

bool path_is_dir(filesystem::path path) {
    return filesystem::is_directory(path);
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

void draw_matiere() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    cout << (!is_cours_selected ? "\033[33;1;100m" : "") << "\033[2;" << size.ws_col / 2 + 2 << "H Matiere "
         << current_cours.substr(2) << ": \033[0m" << endl;
    int i = 0;
    /*
    for (auto const& [key, val] : list_matieres) {
        cout << "\033["<<i+4<<";" << size.ws_col/2+4 << "H" << (key == current_matiere ? "\033[33m" : "") << key.substr(0, size.ws_col/2-4) << "\033[0m" << endl;  //.substr(1)
        i++;
    }
    0;*/
    int j = 0;
    for (std::vector<filesystem::path>::iterator it = std::begin(list_matieres); it != std::end(list_matieres); ++it) {
        cout << "\033[" << i + 4 << ";" << size.ws_col / 2 + 4 << "H"
             << (*it == current_matiere ? "\033[33m" : "")
             << ((path_is_dir(*it)) ? "+ " : "- ")
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
        string superiorbar(max_len_val + 3, '-');
        string replacorbar(max_len_val + 2, ' ');
        cout << "\033[" << j + 4 << ";" << size.ws_col / 2 + 8 << "H\u250f" << borderutf8 * (max_len_val + 1)
             << "\u2513" << endl;
        for (auto const&[key, val]: path_info) {
            cout << "\033[" << j + 5 << ";" << size.ws_col / 2 + 8 << "H" << replacorbar << "\u2503\033[0m" << endl;
            cout << "\033[" << j + 5 << ";" << size.ws_col / 2 + 8 << "H\u2503 " << key << ": " << val << "\033[0m"
                 << endl;
            j++;
        }
        cout << "\033[" << j + 5 << ";" << size.ws_col / 2 + 8 << "H\u2517" << borderutf8 * (max_len_val + 1)
             << "\u251b" << endl;
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
        list_matieres2.push_back(*it);
    }
    int i = 0;
    set <filesystem::path> set_matieres;

    for (std::vector<filesystem::path>::iterator it = std::begin(list_matieres2);
         it != std::end(list_matieres2); ++it) {
        i++;
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

void execute_file() {
    if (!path_is_dir(current_matiere)) {
        filesystem::path current_path = path;
        // if extention is in other_editors keys
        if (other_editors.find(current_path.extension().string()) != other_editors.end()) {
            string command = other_editors[current_path.extension().string()];
            command.append(" ");
            command.append(current_matiere);
            system(command.c_str());

        } else {
            system((editors[current_editor] + " " + path).c_str());
        }

    } else {
        if (send_confirmation("Etez vous sur de vouloir lancer votre editeur sur ce dossier ?", "Oui lance (y/o)",
                              "Ne pas lancer")) {
            system((editors[current_editor] + " " + current_matiere).c_str());
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

    for (int i = 0; i < size.ws_row - 2; i++) {
        cout << "\033[107;30m\033[" << i + 1 << ";" << size.ws_col / 2 << "H" << "|" << "\033[0m";
    }
    draw_cours();
    if (current_cours[0] == '+') {
        draw_matiere();
    } else {
        draw_cours_content();
    }
    // Draw notification
    int i = size.ws_row - 2;
    for (auto &notification: list_notifications) {
        //TODO: TOFIX: Plus stylax
        cout << "\033[107;30m\033[" << i << ";" << 4 << "H" << notification.first << "  " << notification.second.first
             << "  " << notification.second.second << "\033[0m";
        i--;
    }
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

void update() {
    path = current_matiere;// + "   executable = " + ( !path_is_dir(current_matiere) ? "true" : "false");
    check_notifications();

}


void delete_current_path() {
    if (path_is_dir(path)) { // TODO
        if (send_confirmation("Voulez vous vraiment supprimer le dossier " + path + " ?", "Oui", "Non")) {
            filesystem::remove_all(path);
        }
    } else {
        if (send_confirmation("Voulez vous vraiment supprimer le éééééééééfichier " + path + " ?", "Oui", "Non")) {
            create_notification("Fichier supprimé", 100);
            filesystem::remove(path);
            reload_matieres();
        }
    }
}

int main(void) {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    int a = 1000;
    reload_cours();
    //desactiver le cursseur
    cout << "\033[?25l";
    load_config();


    while (a--) {
        update();
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
        update();
        // cout << debugvar << endl;  // Je ne sais pour quelle raison mais si on print un string ou un endl, le send_confirmation ne fonctionne pas
        cout << "";

        string key = get_key_(0);
        if (!is_visualisation) {
            if (key == "n") current_editor = (current_editor + 1) % (editors->length() + 1);
            if (key == "i") current_mode = 1;
            if (key == "r") current_mode = 2;
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
            if (key == "q") {
                //Reactiver le curseur
                cout << "\033[?25h";
                return 0;
            }
            if (key == "v") is_visualisation = !is_visualisation;

            if (key == "t") is_toc = !is_toc;
            if (key == "\033\033") current_mode = 0;
            if (key == "\n") execute_file();  // TODO: Execute en fonction de l'extension
            if (key == "h") is_infos = !is_infos;
            if (key == "R") reload_cours();
            if (key == ".") {
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
            if (key == "d") delete_current_path();
            if (key == "e") cout << "Export all" << endl;
            if (key == "c") cout << "Création d'une matière" << endl;
            if (key == "m") cout << "Création d'un cours" << endl;
            if (key == "f") cout << "Fold d'un cours ou matière" << endl;
            if (key == "?") cout << "Affiche les commandes (Ceci...)" << endl;
        } else {
            if (key == "v") is_visualisation = !is_visualisation;
            if (key == "t") {
                is_toc = !is_toc;
            };
        }
    }
    return 0;

}