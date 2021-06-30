/*
 * This file is part of owper-gui - a graphical front-end to owper
 * Copyright (C) 2010-2018 Matthew Morgan
 *
 * Some code was borrowed/modified from the chntpw project
 * Copyright (c) 1997-2011 Petter Nordahl-Hagen
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the License.
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
#include <exception>
#include <unistd.h>
#include <libgen.h>

#include "include/ntreg.h"
#include "include/sam.h"

#include "include/stringManip.h"
#include "include/fileManip.h"
#include "include/owpException.h"
#include "include/samHive.h"

using std::cout;
using std::endl;
using std::string;
using stringManip::stringPrintf;

using namespace owper;

void printUsage(char* progName) {
    cout << progName << " [--testhive] [HIVEPATH]" << endl;
    cout << "\t--testhive - if this option is present, the program will simply test to see if the given directory contains valid registry hives." << std::endl;
    cout << "\t             This may be useful for scanning multiple partitions on a system to find the hives; some systems have \"dummy\" hives in recovery parititions." << std::endl;
    cout << "\t[HIVEPATH] - the directory containing the system registry files (e.g., SAM, SYSTEM, DEFAULT, etc.)\n" << std::endl;
    cout << "Note: if --testhive is supplied, hivepath must be as well" << endl;
}

void updateExecSearchPath();

// returns:
//     0 - success
//     1 - incorrect arguments supplied
//     2 - the file path supplied does not contain at least a SAM file and the test was requested
int main(int argc, char* argv[]) {
    string hiveFilePath = "";
    samHive* sam = NULL;
    if(argc == 2) {
        // if we have 1 argument, we expect it to be a hive path to load
        if(strncmp("--testhive", argv[1], 10) == 0) {
            // --testhive was specified without a path to test
            printUsage(argv[0]);
            return 1;
        }

        hiveFilePath = argv[1];
    } else if(argc == 3) {
        // if we have 2 arguments, we expect 1 to be the --testhive parameter
        // and the other the be the hive path to test
        if(strncmp("--testhive", argv[1], 10) == 0) {
            hiveFilePath = argv[2];
        } else {
            hiveFilePath = argv[1];
        }

        // we have been asked to test a path to see if it contains valid registry files or not
        string samFileName;
        string samFilePath;
        try {
            samFileName = fileManip::findFileCaseInsensitive(hiveFilePath, "sam");
            if(samFileName.empty()) {
                // the SAM file was not found - bail out
                std::cerr << "Could not find a file named SAM in [ " << hiveFilePath << " ]" << std::endl;
                return 2;
            }

            samFilePath = stringPrintf("%s/%s", hiveFilePath.c_str(), samFileName.c_str());
            sam = new samHive(samFilePath.c_str(), NULL);
            delete sam;
            return 0;
        } catch(owpException* e) {
            // this is NOT a sam hive - bail out
            std::cerr << "Found file [ " << samFilePath << " ] but it is not a valid SAM file" << std::endl;
            delete e;
            return 2;
        }
    }

    // We expect the UI-specific executables to be in the same directory with this one,
    // so we add the current directory of this executable (not to be confused with the current working directory)
    // to the PATH.
    try {
        updateExecSearchPath();
    } catch(std::exception* e) {
        std::cout << "Failed to update executable search path: " << e->what() << std::endl;
        delete e;
    }

    if(getenv("DISPLAY") != NULL) {
        std::cout << "Would run GTK GUI" << std::endl;
    } else {
        std::cout << "Would run NCURSES TUI" << std::endl;
    }
    std::cout << "Path: " << getenv("PATH") << std::endl;
}

void updateExecSearchPath() {
    // It is possible that PATH will not be defined, and for now we won't create it in that case, due to the fact
    // that execve() and friends have a default search path that will include some system directories for that situation.
    char* searchPath = getenv("PATH");
    if(searchPath == NULL) {
        throw(new std::runtime_error("PATH is not in the current environment"));
    }

    // get the full path of the current executable
    char exePath[PATH_MAX + 1] = {};
    if(readlink("/proc/self/exe", exePath, PATH_MAX) == -1) {
        throw(new std::runtime_error("Failed to run readlink on /proc/self/exe to get the current executable's full path"));
    }

    char *exeDirName = dirname(exePath);
    size_t newPathSize = strlen(searchPath) + strlen(exeDirName) + 2; // need extra for colon and null
    char updatedPath[newPathSize];
    int pathCharsWritten = snprintf(updatedPath, newPathSize, "%s:%s", searchPath, exeDirName);
    if(pathCharsWritten != (newPathSize - 1)) { // snprintf doesn't count the NULL it writes
        std::cout << "Something weird happened with snprintf.  Wanted to write [ " << newPathSize << " ] chars, but wrote [ " << pathCharsWritten << " ]" << std::endl;
    }

    if(setenv("PATH", updatedPath, 1) != 0) {
        throw(new std::runtime_error("Failed to add current executable's directory to PATH!"));
    }
}
