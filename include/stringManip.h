#ifndef STRING_MANIP_H
#define STRING_MANIP_H

#include <iostream>
#include <vector>
#include <algorithm>

using std::string;
using std::transform;

string stringToLower(string inString);
string findFileCaseInsensitive(string directory, string fileName);

#endif
