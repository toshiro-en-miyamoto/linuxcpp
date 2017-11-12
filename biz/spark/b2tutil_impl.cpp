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

    static regex stem_pat {"^[Uu][_-]([[:xdigit:]]{4})$"};
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
        if(!valid_unicode(codepoint)) codepoint = 0;
    } catch(...) {
        // invalid_argument
        // out_of_range
        codepoint = 0;
    }

    return codepoint;
}

bool valid_unicode(int codepoint)
{
    using namespace std;
    static vector<bool> v(0x10000);

    static bool initialized = false;
    if(initialized)
        return v[codepoint];

    // initialize the vector v
    initialized = true;
    for(auto c : v) c = false;

    // 0020-007F Basic Latin
    for(auto i=0x0020; i<=0x007F; i++) v[i] =  true; 
    // 00A0-00FF Latin-1 supplements
    for(auto i=0x00A0; i<=0x00FF; i++) v[i] =  true;
    // 0100-017F Latin Extended-A
    for(auto i=0x0100; i<=0x017F; i++) v[i] =  true;
    // 0180-024F Latin Extended-B
    for(auto i=0x0180; i<=0x024F; i++) v[i] =  true;
    // 0250-02AF IPA Extensions
    for(auto i=0x0250; i<=0x02AF; i++) v[i] =  true;
    // 02B0-02FF Spacing Modifier Letters
    for(auto i=0x02B0; i<=0x02FF; i++) v[i] =  true;
    // 0370-03FF Greek and Coptic
    for(auto i=0x0370; i<=0x03FF; i++) v[i] =  true;
    // 0400-04FF Cyrillic
    for(auto i=0x0400; i<=0x04FF; i++) v[i] =  true;
    // 1E00-1EFF Latin Extended Additional
    for(auto i=0x1E00; i<=0x1EFF; i++) v[i] =  true;
    // 2000-206F General Punctuation
    for(auto i=0x2000; i<=0x206F; i++) v[i] =  true;
    // 2070-209F Superscripts and Subscripts
    for(auto i=0x2070; i<=0x209F; i++) v[i] =  true;
    // 20A0-20CF Currency Symbols
    for(auto i=0x20A0; i<=0x20CF; i++) v[i] =  true;
    // 2100-214F Letter-like Symbols
    for(auto i=0x2100; i<=0x214F; i++) v[i] =  true;
    // 2150-218F Number Forms
    for(auto i=0x2150; i<=0x218F; i++) v[i] =  true;
    // 2190-21FF Arrows
    for(auto i=0x2190; i<=0x21FF; i++) v[i] =  true;
    // 2200-22FF Mathematical Operators
    for(auto i=0x2200; i<=0x22FF; i++) v[i] =  true;
    // 2300-23FF Miscellaneous Technical
    for(auto i=0x2300; i<=0x23FF; i++) v[i] =  true;
    // 2460-24FF Enclosed Alphanumeric
    for(auto i=0x2460; i<=0x24FF; i++) v[i] =  true;
    // 2500-257F Box Drawing
    for(auto i=0x2500; i<=0x257F; i++) v[i] =  true;
    // 2580-259F Block Elements
    for(auto i=0x2580; i<=0x259F; i++) v[i] =  true;
    // 25A0-25FF Geometric Shapes
    for(auto i=0x25A0; i<=0x25FF; i++) v[i] =  true;
    // 2600-26FF Miscellaneous Symbols
    for(auto i=0x2600; i<=0x26FF; i++) v[i] =  true;
    // 2700-27BF Dingbats
    for(auto i=0x2700; i<=0x27BF; i++) v[i] =  true;
    // 3000-303F CJK Symbols and Punctuation
    for(auto i=0x3000; i<=0x303F; i++) v[i] =  true;
    // 3040-309F Hiragana
    for(auto i=0x3040; i<=0x309F; i++) v[i] =  true;
    // 30A0-30FF Katakana
    for(auto i=0x30A0; i<=0x30FF; i++) v[i] =  true;
    // 3200-32FF Enclosed CJK Letters and Months
    for(auto i=0x3200; i<=0x32FF; i++) v[i] =  true;
    // 3300-33FF CJK Compatibility
    for(auto i=0x3300; i<=0x33FF; i++) v[i] =  true;
    // F900-FAFF CJK Compatibility Ideographs
    for(auto i=0xF900; i<=0xFAFF; i++) v[i] =  true;
    // FB00-FB4F Alphabetic Presentation Forms
    for(auto i=0xFB00; i<=0xFB4F; i++) v[i] =  true;
    // FF00-FFEF Halfwidth and Fullwidth Forms
    for(auto i=0xFF00; i<=0xFFEF; i++) v[i] =  true;
    // 4E00-9FEA CJK Unified Ideographs
    for(auto i=0x4E00; i<=0x9FEA; i++) v[i] =  true;

    return v[codepoint];
}
