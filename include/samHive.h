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

#ifndef SAM_HIVE_H
#define SAM_HIVE_H

#include <iostream>
#include <vector>
#include "include/hive.h"
#include "include/stringManip.h"
#include "include/sam.h"
#include "include/samUser.h"

using std::vector;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

namespace owper {
    class samHive : public hive {
    private:
    public:
        vector<samUser*> userList;

        void     loadUserList();
        int      getUserRID(char* userName);
        samUser* getSamUser(int rid);
        string   getUserValue(char* dataBuffer, int valueOffset, int valueLength);
        bool     hasValidVStructData(ntreg::keyval *vValue);
        bool     hasValidUserName(int userNameOffset, int userNameLength, int vStructLength);
        bool     hasValidFullName(int fullNameOffset, int fullNameLength, int vStructlength);
    public:
        samHive(const char* fileName, int hiveMode = HMODE_RW);

        vector<string> getUserList();
    };

}

#endif
