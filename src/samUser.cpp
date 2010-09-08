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
#include "include/samUser.h"

namespace owper {
    samUser::samUser(ntreg::keyval *inVStructRegValue) {
        vStruct = (struct ntreg::user_V *)((char*)(&inVStructRegValue->data));
        char* vBuffer = (char*)&(inVStructRegValue->data);
        int userNameOffset = vStruct->username_ofs;
        int userNameLength = vStruct->username_len;
        int fullNameOffset = vStruct->fullname_ofs;
        int fullNameLength = vStruct->fullname_len;
        int vStructLength = inVStructRegValue->len;

        if(!hasValidUserName(userNameOffset, userNameLength, vStructLength)) {
            FREE(vStruct);
            throw(new owpException("VStruct has invalid user name field"));
        }

        if(!hasValidFullName(fullNameOffset, fullNameLength, vStructLength)) {
            FREE(vStruct);
            throw(new owpException("VStruct has invalid full name field"));
        }

        userName = this->getUserValue(vBuffer, userNameOffset, userNameLength);
        fullName = this->getUserValue(vBuffer, fullNameOffset, fullNameLength);
    }

    /**
     * Takes a keyval struct and decides whether it contains a valid VStruct
     * @param keval vValue The registry value to test
     * @return bool
     */
    bool samUser::hasValidVStructData(ntreg::keyval *vValue) {
        if(!vValue) {
            return false;
        }

        //too short
        if(vValue->len < 0xcc) {
            return false;
        }

        return true;
    }

    bool samUser::hasValidUserName(int userNameOffset, int userNameLength, int vStructLength) {
        if(userNameLength <= 0 || //username cannot have 0 length
           userNameLength > vStructLength ||
           userNameOffset <= 0 ||
           userNameOffset >= vStructLength) {
            return false;
        }

        return true;
    }

    bool samUser::hasValidFullName(int fullNameOffset, int fullNameLength, int vStructLength) {
        if(fullNameLength < 0 || //fullname can have 0 length
           fullNameLength > vStructLength ||
           fullNameOffset <= 0 ||
           fullNameOffset >= vStructLength) {
            return false;
        }

        return true;
    }

    string samUser::getUserValue(char* dataBuffer, int valueOffset, int valueLength) {
        valueOffset += 0xCC; //chntpw says we need to do this
                             //something about the offset being relative to the pointers


        char value[128];
        binaryManip::unicodeToAscii(dataBuffer + valueOffset, value, valueLength);

        return (string)value;
    }
}
