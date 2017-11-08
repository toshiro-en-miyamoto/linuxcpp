#include "b2tedit.h"
#include <iostream>

void ttedit_convert(const std::vector<Bmp_file>& v, const fs::path& ttf_path)
{
    for(auto b : v)
        if(b.codepoint != 0)
            std::cout << "Info: processing " << b.path << std::endl;
        else
            std::cout << "Warning: skipping " << b.path << std::endl;

}