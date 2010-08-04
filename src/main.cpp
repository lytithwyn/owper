/*
 * owper - a graphical version of the well known chntpw password changer
 * Copyright (C) 2010 Matthew Morgan
 *
 * Much of this program is modified from chntpw by Petter Nordahl-Hagen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <dirent.h>
#include <vector>
#include <algorithm>

extern "C"
{
    #include "include/ntreg.h"
    #include "include/sam.h"
}

#include "include/stringManip.h"
#include "include/fileManip.h"

using fileManip::findFileCaseInsensitive;

using std::cout;
using std::endl;
using std::string;

int main(int argc, char* argv[]){
    string testDir = "/media/disk/WINDOWS/system32/config/";
    string samFile = findFileCaseInsensitive(testDir, "sam");
    string systemFile = findFileCaseInsensitive(testDir, "system");

    cout << "Found SAM file: " << samFile << endl;
    cout << "Found SYSTEM file: " << systemFile << endl;

    return 1;

}
