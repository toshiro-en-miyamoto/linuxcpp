#include "message.h"
#include "mailfile.h"
#include <iostream>
#include <map>
#include <string>

using namespace my::MailLib;
using namespace std;

int main()
{
    Mail_file mfile {"mailfile.txt"};
    multimap<string, const Message*> sender;

    for(const auto& m : mfile) {
        string s;
        if(find_from_addr(&m, s))
            sender.insert(make_pair(s, &m));
    }

    auto pp = sender.equal_range("John Doe <jdoe@machine.example>");
    for(auto p = pp.first; p != pp.second; ++p)
        cout << find_subject(p->second) << endl;

    return 0;
}