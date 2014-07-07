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
        return this->getDword(0, (char*)ridPath.c_str(), TPF_VK_EXACT|TPF_VK_SHORT);
    }

    samUser* samHive::getSamUser(int rid) {
        string vValuePath = stringPrintf("\\SAM\\Domains\\Account\\Users\\%08X\\V",rid);
        ntreg::keyval *vValue = this->copyValueToBuffer(NULL, 0, (char*)vValuePath.c_str(), REG_BINARY, TPF_VK_EXACT);

        samUser *newSamUser;
        try{
            newSamUser = new samUser(vValue, vValuePath);
        }catch(owpException e) {
            cerr << e.formattedMessage;
            newSamUser = NULL;
        }

        return newSamUser;
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

    /**
     * Merges changes made with samUser objects back into the hive in memory
     * @return bool Whether or not all changes were merged
     */
    bool samHive::mergeChangesToHive() {
        bool allSuccessful = true;
        for(unsigned int i = 0; i < userList.size(); i++) {
            if(userList.at(i)->needsToSave()) {
                ntreg::keyval *keyValue = userList.at(i)->getVStructRegValue();
                string path = userList.at(i)->getVStructPath().c_str();
                int size = copyBufferToValue(keyValue, 0, (char*)path.c_str(), VAL_TYPE_REG_BINARY, TPF_VK_EXACT);

                if(size < 1) {
                    allSuccessful = false;
                } else {
                    userList.at(i)->hasSaved();
                }

                cout << stringPrintf("Merging into %s: wrote %d bytes", path.c_str(), size) << endl;
            }
        }

        regHive->state |= HMODE_DIRTY;
        return allSuccessful;
    }
}
