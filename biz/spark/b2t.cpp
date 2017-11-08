#include "b2tutil.h"
#include "b2tedit.h"
#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{
    if(argc != 3) {
        std::cerr << "Usage: bmp2ttf bmp_dir ttf_file" << std::endl;
        return -1;
    }

    const fs::path bmp_path{argv[1]};
    const fs::path ttf_path{argv[2]};

    if(is_bmp_path_valid(bmp_path) == false) return -1;    
    if(is_ttf_path_valid(ttf_path) == false) return -1;    

    const std::vector<Bmp_file>& bmp_files = bmp_files_in(bmp_path);
    ttedit_convert(bmp_files, ttf_path);
}