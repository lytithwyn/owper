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
        string         vStructPath;
        string         fStructPath;
        string         msAccount;
        ntreg::keyval *vStructRegValue;
        ntreg::keyval *fStructRegValue;
        ntreg::user_V *vStruct;
        ntreg::user_F *fStruct;
        bool           hasBlankPassword;
        bool           regDataChanged;

        bool   hasValidVStructData(ntreg::keyval *vValue);
        bool   hasValidUserName(int userNameOffset, int userNameLength, int vStructLength);
        bool   hasValidFullName(int fullNameOffset, int fullNameLength, int vStructlength);
        string getUserValue(char* dataBuffer, int valueOffset, int valueLength);

    public:
        samUser(ntreg::keyval *inVStructRegValue, string inVStructPath, ntreg::keyval *inFStructRegValue, string inFStructPath, string msAccount = "");
        ~samUser();
        void blankPassword();

        string getFullName() const
        {
            return fullName;
        }

        string getUserName() const
        {
            return userName;
        }

        string getMSAccount() const
        {
            return msAccount;
        }

        bool passwordIsBlank() const
        {
            return hasBlankPassword;
        }

        bool accountIsDisabled() const;

        void enableAccount();
        void disableAccount();

        bool needsToSave() const
        {
            return regDataChanged;
        }

        void hasSaved() {
            regDataChanged = false;
        }

        string getVStructPath() const
        {
            return vStructPath;
        }

        ntreg::keyval *getVStructRegValue() {
            return vStructRegValue;
        }

        string getFStructPath() const
        {
            return fStructPath;
        }

        ntreg::keyval *getFStructRegValue() {
            return fStructRegValue;
        }
    };
}

#endif
