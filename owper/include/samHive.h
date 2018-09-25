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

#ifndef SAM_HIVE_H
#define SAM_HIVE_H

#include <iostream>
#include <vector>
#include <openssl/md5.h>
#include <openssl/rc4.h>
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
        vector<samUser*> userList;
        unsigned char* hashedBootKey;

        unsigned char* generateHashedBootKey(unsigned char* bootKey);
        unsigned char* getGlobalFValue();
        void           loadUserList();
        int            getUserRID(char* userName);
        samUser*       getSamUser(int rid);
        string         getUserValue(char* dataBuffer, int valueOffset, int valueLength);
    public:
        samHive(const char* fileName, unsigned char* bootKey, int hiveMode = HMODE_RW);
        ~samHive();
        vector<samUser*> getUserList(){ return userList; };
        bool     mergeChangesToHive();
        void clearPassword(unsigned int userIndex);
    };

}

#endif
