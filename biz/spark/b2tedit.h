#ifndef B2TEDIT_H
#define B2TEDIT_H

#include "b2tutil.h"
#include <vector>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void ttedit_convert(const std::vector<Bmp_file>&, const fs::path&);
void ttedit_convert(const std::vector<Bmp_file, std::allocator<Bmp_file>>&, const fs::path&);

#endif