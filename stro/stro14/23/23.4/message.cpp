#include "message.h"

namespace my {
namespace MailLib {

int is_prefix(const string& s, const string& p)
{
    int n = p.size();
    if(string(s,0,n) == p) return n;
    return 0;
}

bool find_from_addr(const Message *m, string& s)
{
    for(Line_iter p = m->begin(); p != m->end(); ++p)
        if(int n = is_prefix(*p, "From: ")) {
            s = string(*p,n);
            return true;
        }

    return false;
}

string find_subject(const Message* m)
{
    for(Line_iter p = m->begin(); p != m->end(); ++p)
        if(int n = is_prefix(*p, "Subject: "))
            return string(*p, n);
    return "";
}

} // namespace MailLib
} // namespace my