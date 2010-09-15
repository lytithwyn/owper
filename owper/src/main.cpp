/*
 * This file is part of owper-gui - a graphical front-end to owper
 * Copyright (C) 2010 Matthew Morgan
 *
 * Some code was borrowed/modified from the chntpw project
 * Copyright (c) 1997-2007 Petter Nordahl-Hagen
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

int main(int argc, char* argv[]) {
    string testHive = "/media/disk/WINDOWS/system32/config/SAM";
    if(argc == 2) {
        testHive = argv[1];
    }

    try {
        owper::samHive *myHive = new samHive(testHive.c_str());

        cout << "Users:\n";
        cout << "----------------------------------------\n";
        cout << "User                          Password\n";
        cout << "----------------------------------------\n";

        int testUserIndex1 = -1;
        int testUserIndex2 = -1;

        for(unsigned int i = 0; i < myHive->getUserList().size(); i++) {
            samUser *currentUser = myHive->getUserList().at(i);

            cout << stringPrintf("Found user: %-17s %s",
                    currentUser->getUserName().c_str(),
                    ((currentUser->passwordIsBlank())?("Blank"):("Set"))) << endl;

            if(currentUser->getUserName() == "Tracy") {
                testUserIndex1 = i;
            }

            if(currentUser->getUserName() == "Bob") {
                testUserIndex2 = i;
            }
        }

        cout << endl;

        cout << "Clearing Tracy's password" << endl;
        myHive->getUserList().at(testUserIndex1)->blankPassword();

        cout << "Merging all changes into hive in memory:" << endl;
        if(myHive->mergeChangesToHive()) {
            cout << "All changes successful!\n" << endl;
        } else {
            cout << "Some changes did not merge.\n" << endl;
        }

        cout << "Saving hive to file:" << endl;
        if(myHive->writeHiveToFile()) {
            cout << "Success!" << endl;
        } else {
            cout << "Failed!" << endl;
        }

        cout << "\nClearing Bob's password" << endl;
        myHive->getUserList().at(testUserIndex2)->blankPassword();

        cout << "Merging all changes into hive in memory:" << endl;
        if(myHive->mergeChangesToHive()) {
            cout << "All changes successful!\n" << endl;
        } else {
            cout << "Some changes did not merge.\n" << endl;
        }

        cout << "Saving hive to file:" << endl;
        if(myHive->writeHiveToFile()) {
            cout << "Success!" << endl;
        } else {
            cout << "Failed!" << endl;
        }

        for(unsigned int i = 0; i < myHive->getUserList().size(); i++) {
            samUser *currentUser = myHive->getUserList().at(i);

            cout << stringPrintf("Found user: %-17s %s",
                    currentUser->getUserName().c_str(),
                    ((currentUser->passwordIsBlank())?("Blank"):("Set"))) << endl;
        }
    }catch(exception& e) {
        cout << e.what() << endl;
    }

    return 1;

}
