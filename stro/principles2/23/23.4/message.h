#ifndef MY_MAILLIB_MESSAGE_H
#define MY_MAILLIB_MESSAGE_H

#include <string>
#include <vector>

using namespace std;

namespace my {
namespace MailLib {

typedef vector<string>::const_iterator Line_iter;

class Message {
    Line_iter first;
    Line_iter last;
public:
    Message(Line_iter p1, Line_iter p2) : first{p1}, last{p2} {}
    Line_iter begin() const { return first; }
    Line_iter end() const { return last; }
};

bool find_from_addr(const Message*, string&);
string find_subject(const Message*);

} // namespace MailLib
} // namespace my

#endif