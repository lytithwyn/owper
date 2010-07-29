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
}

using std::cout;
using std::endl;
using std::string;
using std::transform;

/**
 * Find a file in the specified directory, ignoring case.
 * @param string directory The directory in which to search
 * @param string fileName The file name to look for (case ignored)
 * @return string The file name with the proper case (as it exists in the filesystem) or an empty string if it wasn't found.
 */
string findFileCaseInsensitive(string directory, string fileName){
	DIR *dirTest;
	dirent *dirEntry;

	//make sure "directory" exists and is a directory
	if(!(dirTest = opendir(directory.c_str()))){
		return ""; //not a directory
	}

	//convert the filename we were given to lower case before searching
	string fileNameLower = fileName; // to fix freakiness in transform
    transform(fileName.begin(), fileName.end(), fileNameLower.begin(),
        static_cast < int(*)(int) > (tolower));

    string testFileName;
    string testFileNameLower;
    while((dirEntry = readdir(dirTest)))
    {
        testFileName.erase();
        testFileNameLower.erase();

        testFileName = dirEntry->d_name;
        testFileNameLower = testFileName; // to fix freakiness in transform

        transform(testFileName.begin(), testFileName.end(), testFileNameLower.begin(),
            static_cast < int(*)(int) > (tolower));

        if(testFileNameLower == fileNameLower)
            return testFileName;
    }

    return "";

}

int main(int argc, char* argv[]){
	string testDir = "/media/disk/WINDOWS/system32/config/";
	string samFile = findFileCaseInsensitive(testDir, "sam");
	string systemFile = findFileCaseInsensitive(testDir, "system");

	cout << "Found SAM file: " << samFile << endl;
	cout << "Found SYSTEM file: " << systemFile << endl;

	return 1;

}
