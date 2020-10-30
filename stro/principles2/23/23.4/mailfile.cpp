#include "mailfile.h"
#include <string>
#include <iostream>
#include <fstream>

namespace my {
namespace MailLib {

using namespace std;

Mail_file::Mail_file(const string& n)
{
    ifstream in {n};
    if(!in) {
        cerr << "no " << n << endl;
        std::exit(1);
    }

    for(string s; getline(in, s); )
        lines.push_back(s);

    auto first = lines.begin();
    for(auto p = lines.begin(); p != lines.end(); ++p) {
        if(*p == "----") {
            m.push_back(Message(first, p));
            first = p + 1;
        }
    }
}

} // namespace MailLib
} // namespace my