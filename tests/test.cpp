//
// Created by ay on 2/2/22.
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
#include <regex>        // sort by number
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

bool isdigits(string s){
         for(char c:s){
                 if(!isdigit(c)){
                         return false;
                 }
         };
         return true;
 }
int main() {
	string ial("1.1.Raisinoment_par_recurence.md");
	string il1 = ial.substr(0, ial.find("."));

	if(isdigits(il1)){
		cout << stoi(il1)+1 << endl;
	}else{
		cout << "e:" << il1 << endl;
	}

	return 0;
}
