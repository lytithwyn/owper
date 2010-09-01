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
#include "include/hive.h"

using std::cout;
using std::endl;
using std::string;
using stringManip::stringPrintf;

using namespace owper;

int main(int argc, char* argv[]) {
    string testHive = "/media/disk/WINDOWS/system32/config/SAM";

    try {
        owper::hive *myHive = new hive(testHive.c_str());

        char s[200];
        struct ntreg::keyval *v;
        int rid;
        int count = 0, countri = 0;
        struct ntreg::ex_data ex;
        char admuser[129]="Administrator";

        reg_off nkofs = myHive->travPath(0,(char*)"\\SAM\\Domains\\Account\\Users\\Names\\",0);

        if (!nkofs) {
          printf("list_users: Cannot find usernames in registry! (is this a SAM-hive?)\n");
          return(1);
        }else {
            printf("Nkofs: %d\n", nkofs);
        }

        while ((myHive->getNextSubKey(nkofs+4, &count, &countri, &ex) > 0)) {
            /* Extract the value out of the username-key, value is RID  */
            snprintf(s,180,"\\SAM\\Domains\\Account\\Users\\Names\\%s\\@",ex.name);
            rid = myHive->getDword(0, s);
            if (rid == 500) strncpy(admuser,ex.name,128); /* Copy out admin-name */

            /*    printf("name: %s, rid: %d (0x%0x)\n", ex.name, rid, rid); */

            /* Now that we have the RID, build the path to, and get the V-value */
            snprintf(s,180,"\\SAM\\Domains\\Account\\Users\\%08X\\V",rid);
            v = myHive->copyValueToBuffer(NULL, 0, s, REG_BINARY);
            if (!v) {
              printf("Cannot find value <%s>\n",s);
              return(1);
            }

            if (v->len < 0xcc) {
              printf("list_users: Value <%s> is too short (only %d bytes) to be a SAM user V-struct!\n",
                 s, v->len);
            } else {
                struct ntreg::user_V *v2;
                v2 = (struct ntreg::user_V *)((char*)(&v->data));
                char* vp = (char*)&(v->data);
                int username_offset = v2->username_ofs;
                int username_len = v2->username_len;
                int vlen = v->len;

                if(username_len <= 0 || username_len > vlen ||
                      username_offset <= 0 || username_offset >= vlen) {
                    printf("Can't get username!\n");
                }

                char username[128];
                username_offset += 0xCC;
                myHive->unicodetoAscii(vp + username_offset,username,username_len);

                printf("Found username: %s\n", username);


            }
            FREE(v);
            FREE(ex.name);
        }

    }catch(exception& e) {
        cout << e.what() << endl;
    }

    return 1;

}
