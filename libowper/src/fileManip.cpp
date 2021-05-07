/*
 * This file is part of owper - a c++ windows password changing library based on chntpw
 * Copyright (C) 2010-2018 Matthew Morgan
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
        string foundFileName = "";
        while((dirEntry = readdir(dirTest))) {
            testFileName.erase();
            testFileNameLower.erase();

            testFileName = dirEntry->d_name;
            testFileNameLower = stringToLower(testFileName);

            if(testFileNameLower == fileNameLower) {
                foundFileName = testFileName;
                break;
            }
        }

        closedir(dirTest);
        return foundFileName;

    }

    string buildUniqueBackupFileName(string originalName) {
        struct stat statBuf;
        string backupFileName;
        int backupIndex = 0;
        int statReturn = 0;

        // find an unused file name for the backup
        while(statReturn == 0) {
            backupFileName = stringPrintf("%s.bak%d", originalName.c_str(), backupIndex);
            statReturn = stat(backupFileName.c_str(), &statBuf);
            ++backupIndex;
        }

        return backupFileName;
    }

    bool copyFile(string sourceFileName, string destFileName) {
        struct stat statBuf;
        if((stat(sourceFileName.c_str(), &statBuf)) != 0) {
            return false;
        }

        int sourceFileDesc = open(sourceFileName.c_str(), O_RDONLY);
        if(sourceFileDesc == -1) {
            perror("Failed to open source file for reading!");
            std::cerr << sourceFileName << std::endl;
            return false;
        }

        int destFileDesc = creat(destFileName.c_str(), 00700);
        if(destFileDesc == -1) {
            perror("Failed to open dest file for writing!");
            std::cerr << sourceFileName << std::endl;
            close(sourceFileDesc);
            return false;
        }

        const int RB_SIZE = 512*100;
        char readBuf[RB_SIZE];
        int bytesRead = 0;
        int bytesWritten = 0;
        int totalBytesTransferred = 0;
        while(true) {
            bytesRead = read(sourceFileDesc, readBuf, RB_SIZE);
            if(bytesRead == -1) {
                perror("Failed to read from source file!");
                std::cerr << sourceFileName << std::endl;
                break;
            }

            bytesWritten = write(destFileDesc, readBuf, bytesRead);
            if(bytesWritten < bytesRead) {
                std::cerr << "Incomplete write to dest file!\n" << destFileName << std::endl;
                break;
            }

            totalBytesTransferred += bytesRead;
            if(bytesRead < RB_SIZE) {
                break;
            }
        }

        close(sourceFileDesc);
        close(destFileDesc);

        if(totalBytesTransferred < statBuf.st_size) {
            std::cerr << "Failed to completely transfer file contents!\n" << sourceFileName << " > " << destFileName << std::endl;
            return false;
        }

        return true;
    }
}
