#include <iostream>
#include <string>
#include <fstream>
#include <regex>
using namespace std;

int main()
{
    ifstream in {"uszipcode.txt"};
    if(!in) cerr << "failed to open" << endl;

    regex pat {R"(\w{2}\s*\d{5}(-\d{4})?)"};
    // regex pat {"\\w{2}\\s*\\d{5}(-\\d{4})?"};

    int lineno = 0;
    for(string line; getline(in, line); ) {
        ++lineno;
        smatch matches;
        if(regex_search(line, matches, pat)) {
            cout << lineno << " : " << matches[0] << endl;
            if(1<matches.size() && matches[1].matched)
                cout << "\t: " << matches[1] << endl;
        }
    }
}