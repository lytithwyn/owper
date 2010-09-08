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

#ifndef SAM_USER_H
#define SAM_USER_H

#include <iostream>
#include <cstdlib>
#include <string>

#include "include/ntreg.h"
#include "include/sam.h"
#include "include/binaryManip.h"
#include "include/owpException.h"

using std::string;

namespace owper {
    class samUser {
    private:
        string         userName;
        string         fullName;
        ntreg::keyval *vStructRegValue;
        ntreg::user_V *vStruct;

        bool   hasValidVStructData(ntreg::keyval *vValue);
        bool   hasValidUserName(int userNameOffset, int userNameLength, int vStructLength);
        bool   hasValidFullName(int fullNameOffset, int fullNameLength, int vStructlength);
        string getUserValue(char* dataBuffer, int valueOffset, int valueLength);

    public:
        samUser(ntreg::keyval *inVStructRegValue);

        string getFullName() const
        {
            return fullName;
        }

        string getUserName() const
        {
            return userName;
        }
    };
}

#endif
