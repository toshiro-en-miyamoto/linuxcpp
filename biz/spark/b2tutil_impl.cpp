#include "b2tutil_impl.h"
#include <regex>
#include <string>

std::vector<Bmp_file>& bmp_file_vector()
{
    static std::vector<Bmp_file> v;
    return v;
}

int codepoint_of_bmp_filename(const fs::path& p)
{
    // for example, given the path
    //    p = "c:/foo/hello.txt",
    // then, we have
    //    p.root_name() == "c:",
    //    p.root_directory() == "/",
    //    p.relative_path() == "foo/hello.txt",
    //    p.stem() == "hello", and 
    //    p.extension() == ".txt".
    using namespace std;

    static regex stem_pat {"^[Uu][_-]([[:xdigit:]]{4,5})$"};
    smatch stem_match;
    const string& stem = p.stem().string();
    

    static regex ext_pat {"\\.[Bb][Mm][Pp]"};
    smatch ext_match;
    const string& ext = p.extension().string();

    if( !regex_search(ext, ext_match, ext_pat) ||
        !regex_search(stem, stem_match, stem_pat))
    {
        // the filename violates the naming rule
        return 0;
    }

    int codepoint = 0;
    try {
        codepoint = stoi(stem_match[1], nullptr, 16);
        if(0x1FFFF < codepoint) codepoint = 0;
    } catch(...) {
        // invalid_argument
        // out_of_range
        codepoint = 0;
    }

    return codepoint;
}
