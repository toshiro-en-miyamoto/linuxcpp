#ifndef B2TUTIL_H
#define B2TUTIL_H

#include <vector>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

/**
 * vector<Bmp_file> holds every single file in bmp_dir,
 * even if it doesn't follow file naming convention.
 */
struct Bmp_file {
    fs::path path;          // path/to/U-xxxxx.bmp
    int codepoint;          // xxxxx, 0 if invalid file name
};

/**
 * Returns a vector of Bmp_file struct.
 * The vector<Bmp_file> holds every single file in bmp_dir,
 * even if it doesn't follow file naming convention.
 */
std::vector<Bmp_file>& bmp_files_in(const fs::path&);

/**
 * Returns true if
 * 1. there exists the path, and
 * 2. the path is a directory
 */
bool is_bmp_path_valid(const fs::path&);

/*
 * Check the path to the TTF file to see if
 * 1. there exists the path, and
 * 2. the path is a regular file
 */
bool is_ttf_path_valid(const fs::path&);

#endif