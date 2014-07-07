/*
 * This file is part of owper - a c++ windows password changing library based on chntpw
 * Copyright (C) 2010-2013 Matthew Morgan
 *
 * Some code was borrowed/modified from the chntpw project
 * Copyright (c) 1997-2011 Petter Nordahl-Hagen
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
 *
 */

#include "include/fileManip.h"

namespace fileManip {
    /**
     * Find a file in the specified directory, ignoring case.
     * @param string directory The directory in which to search
     * @param string fileName The file name to look for (case ignored)
     * @return string The file name with the proper case (as it exists in the filesystem) or an empty string if it wasn't found.
     */
    string findFileCaseInsensitive(string directory, string fileName) {
        DIR *dirTest;
        dirent *dirEntry;

        //make sure "directory" exists and is a directory
        if(!(dirTest = opendir(directory.c_str()))) {
            return ""; //not a directory
        }

        //convert the filename we were given to lower case before searching
        string fileNameLower = stringToLower(fileName);

        string testFileName;
        string testFileNameLower;
        while((dirEntry = readdir(dirTest))) {
            testFileName.erase();
            testFileNameLower.erase();

            testFileName = dirEntry->d_name;
            testFileNameLower = stringToLower(testFileName);

            if(testFileNameLower == fileNameLower)
                return testFileName;
        }

        return "";

    }
}
