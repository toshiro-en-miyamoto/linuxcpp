#include "b2tutil.h"
#include "b2tutil_impl.h"
#include <iostream>

/**
 * Returns a vector of Bmp_file struct.
 * The vector<Bmp_file> holds every single file in bmp_dir,
 * even if it doesn't follow file naming convention.
 */
std::vector<Bmp_file>& bmp_files_in(const fs::path& bmp_path)
{
    // work with files in the bmp_dir
    using namespace std;

    error_code ec1;
    for(auto entry : fs::recursive_directory_iterator(bmp_path, ec1)) {
        error_code ec2;
        fs::file_status s = fs::status(entry, ec2);
        if(fs::is_directory(s)) {
            // std::cout << "Info: directory  " << entry << std::endl;
        } else if(fs::is_regular_file(s)) {
            bmp_file_vector()
            .push_back(Bmp_file{entry, codepoint_of_bmp_filename(entry)});
        } else {
            cerr << "Warning: "
                << entry << " is not a regular file" << endl;
        }
    }

    return bmp_file_vector();
}

/**
 * See b2tutil.h
 */
bool is_bmp_path_valid(const fs::path& p)
{
    std::error_code ec;
    fs::file_status s = fs::status(p, ec);
    // how to check ec?

    if(fs::is_directory(s)) {
        // OK, we proceed to checking permission
    } else if(!fs::exists(s)) {
        std::cerr << "Error: [" << p << "] not found" << std::endl;
        return false;
    } else {
        std::cerr << "Error: [" << p << "] is not a directory" << std::endl;
        return false;
    }

    // do we need to check read permission?

    return true;
}

/**
 * See b2tutil.h
 */
bool is_ttf_path_valid(const fs::path& p)
{
    std::error_code ec;
    fs::file_status s = fs::status(p, ec);
    // how to check ec?

    if(fs::is_regular_file(s)) {
        // OK, we proceed to checking permission
    } else if(!fs::exists(s)) {
        std::cerr << "Error: [" << p << "] not found" << std::endl;
        return false;
    } else {
        std::cerr << "Error: [" << p << "] is not a file" << std::endl;
        return false;
    }

    // do we need to check read permission?

    return true;
}