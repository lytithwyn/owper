#ifndef FILE_MANIP_H
#define FILE_MANIP_H

#include <iostream>
#include <dirent.h>

#include "include/stringManip.h"
using stringManip::stringToLower;

namespace fileManip {
    string findFileCaseInsensitive(string directory, string fileName);
}

#endif
