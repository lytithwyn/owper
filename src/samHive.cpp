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
    }

    vector<string> samHive::getUserList() {
        vector<string> userNames;

        reg_off regKeyOffset = this->travPath(0,(char*)"\\SAM\\Domains\\Account\\Users\\Names\\",0);

        if(!regKeyOffset) {
            cerr << "List_users: Cannot find usernames in registry! (is this a SAM-hive?)" << endl;
            return userNames;
        } else {
            cout << stringPrintf("regKeyOffset: %d\n", regKeyOffset) << endl;
        }

        int subKeyCount = 0;
        int subKeyCountRI = 0;
        struct ntreg::ex_data exData;
        string adminUser;
        while((this->getNextSubKey(regKeyOffset+4, &subKeyCount, &subKeyCountRI, &exData) > 0)) {
            /* Extract the value out of the username-key, value is RID  */
            string ridPath = stringManip::stringPrintf("\\SAM\\Domains\\Account\\Users\\Names\\%s\\@",exData.name);
            int rid = this->getDword(0, (char*)ridPath.c_str());

            if(rid == 500) {
                adminUser = exData.name;
            }

            /* Now that we have the RID, build the path to, and get the V-value */
            string vValuePath = stringPrintf("\\SAM\\Domains\\Account\\Users\\%08X\\V",rid);
            struct ntreg::keyval *vValue = this->copyValueToBuffer(NULL, 0, (char*)vValuePath.c_str(), REG_BINARY);
            if(!vValue) {
                cerr << stringPrintf("List Users: Cannot find value <%s>", vValuePath.c_str()) << endl;
                continue;
            }

            if(vValue->len < 0xcc) {
                cerr << stringPrintf("List Users: <%s> is too short (only %d bytes) to be a SAM user V-struct!\n",
                 vValuePath.c_str(), vValue->len);
            } else {
                ntreg::user_V *userV;
                userV = (struct ntreg::user_V *)((char*)(&vValue->data));
                char* vBuffer = (char*)&(vValue->data);
                int userNameOffset = userV->username_ofs;
                int userNameLength = userV->username_len;
                int vValueLength = vValue->len;

                if(userNameLength <= 0 || userNameLength > vValueLength ||
                        userNameOffset <= 0 || userNameOffset >= vValueLength) {
                    cerr << "List Users: can't get username!" << endl;
                    continue;
                }

                char username[128];
                userNameOffset += 0xCC; //chntwp says we need to offset this address
                this->unicodeToAscii(vBuffer + userNameOffset, username, userNameLength);

                cout << stringPrintf("Found username: %s", username) << endl;
            }

            FREE(vValue);
            FREE(exData.name);
        }

        return userNames;
    }
}
