#ifndef B2TUTIL_IMPL_H
#define B2TUTIL_IMPL_H

#include "b2tutil.h"
#include <vector>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

/**
 * Initializer of vector<Bmp_file>
 */
std::vector<Bmp_file>& bmp_file_vector();

/**
 * Returns the unicode in integer that is encoded in
 * the filename if it follows the naming rule:
 * - stem "^[Uu][-_][:xdigit:]{4}$"
 * - .ext "\.[Bb][Mm][Pp]"
 * Returns 0 if the filename does not follow the rule or
 * [:xdigit:]{4} is not in a valid range of Unicode 10.0.0.
 *
 * The b2tutil supports unicode characters in the range
 *      from U-0001 to U-FFFF.
 *
 * Note that U-00000 is not supported as it is used to
 * indicate 'invalid code point'.
 *
 * [For your reference] Unicode 10.0.0 (June 20, 2017)
 * adds 8,518 characters, for a total of 136,690 characters.
 * These additions include 4 new scripts, for a total of 139
 * scripts, as well as 56 new emoji characters.
 *      Plane 0 (BMP)   0 0000 - 0 FFFF
 *      Plane 1 (SMP)   1 0000 - 1 FFFF
 */
int codepoint_of_bmp_filename(const fs::path&);

/**
 * Returns true if the argument 'codepoint' is in the range of
 * valid unicode defined by JIS.
 */
bool valid_unicode(int);

#endif