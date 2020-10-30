#ifndef MY_MAILLIB_MAILFILE_H
#define MY_MAILLIB_MAILFILE_H

#include "message.h"
#include <vector>
#include <string>
using namespace std;

namespace my {
namespace MailLib {

typedef vector<Message>::const_iterator Mess_iter;

struct Mail_file {
    string name;
    vector<string> lines;
    vector<Message> m;

    Mail_file(const string& n);    // read file n into lines
    Mess_iter begin() const { return m.begin(); }
    Mess_iter end() const { return m.end(); }
};

} // namespace MailLib
} // namespace my

#endif