/*
 * This file is part of owper - a c++ windows password changing library based on chntpw
 * Copyright (C) 2010-2018 Matthew Morgan
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
#include <cstring>

#include <openssl/md5.h>
#include <openssl/rc4.h>
#include <openssl/des.h>

#include "include/ntreg.h"
#include "include/sam.h"
#include "include/binaryManip.h"
#include "include/owpException.h"

using std::string;

namespace owper {
    class samUser {
    private:
        int               rid;
        string            userName;
        string            fullName;
        string            vStructPath;
        string            fStructPath;
        string            msAccount;
        int               nkOffset;
        char*             vBuffer;
        ntreg::keyval*    vStructRegValue;
        ntreg::keyval*    fStructRegValue;
        ntreg::user_V*    vStruct;
        ntreg::user_F*    fStruct;
        unsigned char*    lmHash;
        unsigned char*    ntHash;
        des_key_schedule* keySched1;
        des_key_schedule* keySched2;
        unsigned char*    hashedBootKey;
        bool              regDataChanged;

        bool   hasValidVStructData(ntreg::keyval *vValue);
        bool   hasValidUserName(int userNameOffset, int userNameLength, int vStructLength);
        bool   hasValidFullName(int fullNameOffset, int fullNameLength, int vStructlength);
        string getUserValue(char* dataBuffer, int valueOffset, int valueLength);

        // syskey stuff
        unsigned char* decryptHash(int hashOffset, int hashLength, const char* extraHashInput);
        bool hashIsEmpty(unsigned const char* hash, const char* emptyHashPreset);
        char* hashToString(unsigned const char* hash);
        int compareHash(unsigned const char* hash, const char* stringToCompare);
        void ridToKey1(unsigned long rid, unsigned char deskey[8]);
        void ridToKey2(unsigned long rid, unsigned char deskey[8]);
        void strToDesKey(unsigned char *str, unsigned char *key);

    public:
        samUser(int inRid, int inNKOffset, ntreg::keyval *inVStructRegValue, string inVStructPath, ntreg::keyval *inFStructRegValue, string inFStructPath, unsigned char* inHashedBootKey, string msAccount = "");
        ~samUser();

        string getFullName() const   { return fullName; };
        string getUserName() const   { return userName; };
        string getMSAccount() const  { return msAccount; };
        void clearFullName() { this->fullName = ""; };
        void clearMSAccount() { this->msAccount = ""; };
        bool passwordIsBlank();
        bool accountIsDisabled() const;
        void enableAccount();
        void disableAccount();
        void hasChanges(bool dataChanged){ regDataChanged = dataChanged; };
        bool hasChanges() const { return regDataChanged; };
        void hasSaved() { regDataChanged = false; };
        int getNKOffset() const { return this->nkOffset; };
        string getVStructPath() const { return vStructPath; };
        ntreg::keyval *getVStructRegValue() { return vStructRegValue; };
        string getFStructPath() const { return fStructPath; };
        ntreg::keyval *getFStructRegValue() { return fStructRegValue; };
    };
}

#endif
