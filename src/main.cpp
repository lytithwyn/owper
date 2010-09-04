/*
 * This file is part of owper - a graphical version of the well known chntpw password changer
 * Copyright (C) 2010 Matthew Morgan
 *
 * Some code was borrowed/modified from the chntpw project
 * Copyright (c) 1997-2007 Petter Nordahl-Hagen
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
        cout << "UserName\t\t\tFullName\n";
        cout << "----------------------------------------\n";
        for(unsigned int i = 0; i < myHive->userList.size(); i++) {
            cout << stringPrintf("Found user: %s\t%s",
                    (myHive->userList.at(i))->getUserName().c_str(),
                    (myHive->userList.at(i))->getFullName().c_str()) << endl;
        }
    }catch(exception& e) {
        cout << e.what() << endl;
    }

    return 1;

}
