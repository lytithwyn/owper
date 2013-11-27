/*
 * This file is part of owper - a c++ windows password changing library based on chntpw
 * Copyright (C) 2010-2013 Matthew Morgan
 *
 * Some code was borrowed/modified from the chntpw project
 * Copyright (c) 1997-2011 Petter Nordahl-Hagen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * See file LGPL.txt for the full license.
 *
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
        vector<samUser*> userList;

        void     loadUserList();
        int      getUserRID(char* userName);
        samUser* getSamUser(int rid);
        string   getUserValue(char* dataBuffer, int valueOffset, int valueLength);
    public:
        samHive(const char* fileName, int hiveMode = HMODE_RW);
        vector<samUser*> getUserList(){ return userList; };
        bool     mergeChangesToHive();
    };

}

#endif
