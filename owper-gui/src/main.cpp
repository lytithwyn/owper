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
#include <gtk/gtk.h>

#include "include/ntreg.h"
#include "include/sam.h"

#include "include/stringManip.h"
#include "include/fileManip.h"
#include "include/owpException.h"
#include "include/samHive.h"
#include "include/owperGUI.h"

using std::cout;
using std::endl;
using std::string;
using stringManip::stringPrintf;

using namespace owper;

void printUsage(char* progName) {
    cout << progName << " [--testhive] [HIVEPATH]" << endl;
    cout << "\t--testhive - if this option is present, the program will simply test to see if the given directory contains valid registry hives." << std::endl;
    cout << "\t             This may be useful for scanning multiple partitions on a system to find the hives, where some might \"dummy\" hives in recovery parititions." << std::endl;
    cout << "\t[HIVEPATH] - the directory containing the system registry files (e.g., SAM, SYSTEM, DEFAULT, etc.)\n" << std::endl;
    cout << "Note: if --testhive is supplied, hivepath must be as well" << endl;
}

// returns:
//     0 - success
//     1 - incorrect arguments supplied
//     2 - file supplied is not a sam type file and a check was requested
int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    string hiveFilePath = "";
    samHive* sam = NULL;
    if(argc == 2) {
        // if we have 1 argument, we expect it to be a hive file to load
        if(strncmp("--testhive", argv[1], 10) == 0) {
            // --testhive was specified without a file to test
            printUsage(argv[0]);
            return 1;
        }

        hiveFilePath = argv[1];
    } else if(argc == 3) {
        // if we have 2 arguments, we expect 1 to be the --testhive parameter
        // and the other the be the hive file to load
        if(strncmp("--testhive", argv[1], 10) == 0) {
            hiveFilePath = argv[2];
        } else {
            hiveFilePath = argv[1];
        }

        // we have been asked to test a path to see if it contains valid registry files or not
        try {
            string samFileName = fileManip::findFileCaseInsensitive(hiveFilePath, "sam");
            if(samFileName.empty()) {
                // the SAM file was not found - bail out
                return 2;
            }
            string samFilePath = stringPrintf("%s/%s", hiveFilePath.c_str(), samFileName.c_str());
            sam = new samHive(samFilePath.c_str(), NULL);
            delete sam; // we're going to reload it with the bootkey from SYSTEM anyway
        } catch(owpException* e) {
            // this is NOT a sam hive - bail out
            delete e;
            return 2;
        }
    }

    owperGUI *passwordClearer;
    passwordClearer = new owperGUI(hiveFilePath);

    gtk_main();

    delete passwordClearer;

    return 0;

}
