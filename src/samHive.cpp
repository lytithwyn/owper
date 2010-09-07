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

#include "include/samHive.h"

namespace owper {
    samHive::samHive(const char* fileName, int hiveMode/* = HMODE_RW*/):
            hive(fileName, hiveMode) {
        if(this->getType() != HIVE_TYPE_SAM) {
            this->closeHive();
            throw(new owpException("The filename given does not point to a SAM type hive"));
        }

        this->loadUserList();
    }

    reg_off samHive::getUserRID(char* userName) {
        string ridPath = stringManip::stringPrintf("\\SAM\\Domains\\Account\\Users\\Names\\%s\\@",userName);
        return this->getDword(0, (char*)ridPath.c_str());
    }

    samUser* samHive::getSamUser(int rid) {
        string vValuePath = stringPrintf("\\SAM\\Domains\\Account\\Users\\%08X\\V",rid);
        ntreg::keyval *vValue = this->copyValueToBuffer(NULL, 0, (char*)vValuePath.c_str(), REG_BINARY);

        if(!hasValidVStructData(vValue)) {
            cerr << stringPrintf("List Users: Key %s does not contain valid VStruct Data!\n",
                    vValuePath.c_str());
            FREE(vValue);
            return NULL;
        }

        ntreg::user_V *userV;
        userV = (struct ntreg::user_V *)((char*)(&vValue->data));
        char* vBuffer = (char*)&(vValue->data);
        int userNameOffset = userV->username_ofs;
        int userNameLength = userV->username_len;
        int fullNameOffset = userV->fullname_ofs;
        int fullNameLength = userV->fullname_len;
        int vValueLength = vValue->len;

        if(!hasValidUserName(userNameOffset, userNameLength, vValueLength)) {
            cerr << "VStruct has invalid user name field" << endl;
            FREE(vValue);
            return NULL;
        }

        if(!hasValidFullName(fullNameOffset, fullNameLength, vValueLength)) {
            cerr << "VStruct has invalid full name field" << endl;
            FREE(vValue);
            return NULL;
        }

        string userName = this->getUserValue(vBuffer, userNameOffset, userNameLength);
        string fullName = this->getUserValue(vBuffer, fullNameOffset, fullNameLength);

        FREE(vValue);
        return new samUser(userName, fullName);
    }

    string samHive::getUserValue(char* dataBuffer, int valueOffset, int valueLength) {
        valueOffset += 0xCC; //chntpw says we need to do this
                             //something about the offset being relative to the pointers


        char value[128];
        this->unicodeToAscii(dataBuffer + valueOffset, value, valueLength);

        return (string)value;
    }

    /**
     * Takes a keyval struct and decides whether it contains a valid VStruct
     * @param keval vValue The registry value to test
     * @return bool
     */
    bool samHive::hasValidVStructData(ntreg::keyval *vValue) {
        if(!vValue) {
            return false;
        }

        //too short
        if(vValue->len < 0xcc) {
            return false;
        }

        return true;
    }

    bool samHive::hasValidUserName(int userNameOffset, int userNameLength, int vStructLength) {
        if(userNameLength <= 0 || //username cannot have 0 length
           userNameLength > vStructLength ||
           userNameOffset <= 0 ||
           userNameOffset >= vStructLength) {
            return false;
        }

        return true;
    }

    bool samHive::hasValidFullName(int fullNameOffset, int fullNameLength, int vStructLength) {
        if(fullNameLength < 0 || //fullname can have 0 length
           fullNameLength > vStructLength ||
           fullNameOffset <= 0 ||
           fullNameOffset >= vStructLength) {
            return false;
        }

        return true;
    }

    void samHive::loadUserList() {
        reg_off regKeyOffset = this->travPath(0,(char*)"\\SAM\\Domains\\Account\\Users\\Names\\",0);

        if(!regKeyOffset) {
            throw(new owpException("loadUserList: could not find usernames in registry!"));
        } else {
            cout << stringPrintf("regKeyOffset: %d\n", regKeyOffset) << endl;
        }

        int subKeyCount = 0;
        int subKeyCountRI = 0;
        struct ntreg::ex_data exData;
        string adminUser;
        while((this->getNextSubKey(regKeyOffset+4, &subKeyCount, &subKeyCountRI, &exData) > 0)) {
            //get the RID
            int rid = this->getUserRID(exData.name);

            if(rid == 500) {
                adminUser = exData.name;
            }

            samUser* newSamUser = this->getSamUser(rid);
            if(newSamUser != NULL) {
                this->userList.push_back(newSamUser);
            }

            FREE(exData.name);
        }
    }
}
