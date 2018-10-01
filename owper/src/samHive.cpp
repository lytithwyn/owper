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

#include "include/samHive.h"

namespace owper {
    samHive::samHive(const char* fileName, unsigned char* bootKey, int hiveMode/* = HMODE_RW*/):
            hive(fileName, hiveMode) {
        if(this->getType() != HIVE_TYPE_SAM) {
            this->closeHive();
            throw(new owpException("The filename given does not point to a SAM type hive"));
        }

        if(bootKey) {
            this->hashedBootKey = this->generateHashedBootKey(bootKey);
            delete[] bootKey;
        } else {
            this->hashedBootKey = NULL;
        }

        this->loadUserList();
    }

    reg_off samHive::getUserRID(char* userName) {
        string ridPath = stringManip::stringPrintf("\\SAM\\Domains\\Account\\Users\\Names\\%s\\@",userName);
        return this->getDword(0, (char*)ridPath.c_str(), TPF_VK_EXACT|TPF_VK_SHORT);
    }

    samUser* samHive::getSamUser(int rid) {
        string userNKPath = stringPrintf("\\SAM\\Domains\\Account\\Users\\%08X", rid);
        reg_off userNKOffset = this->travPath(0, (char*)userNKPath.c_str(), 0) + 4;
        string vValuePath = stringPrintf("\\SAM\\Domains\\Account\\Users\\%08X\\V",rid);
        ntreg::keyval *vValue = this->copyValueToBuffer(NULL, 0, (char*)vValuePath.c_str(), REG_BINARY, TPF_VK_EXACT);

        string fValuePath = stringPrintf("\\SAM\\Domains\\Account\\Users\\%08X\\F",rid);
        ntreg::keyval *fValue = this->copyValueToBuffer(NULL, 0, (char*)fValuePath.c_str(), REG_BINARY, TPF_VK_EXACT);

        // this should be null for local accounts, and set for microsoft accounts
        string internetUserNamePath = stringPrintf("\\SAM\\Domains\\Account\\Users\\%08X\\InternetUserName",rid);
        ntreg::keyval *internetUserNameValue = this->copyValueToBuffer(NULL, 0, (char*)internetUserNamePath.c_str(), REG_BINARY, TPF_VK_EXACT);
        string internetUserName = "";
        if(internetUserNameValue != NULL) {
            int inetUserNameLen = internetUserNameValue->len;
            char* inetUserNameCString = new char[inetUserNameLen + 1];
            inetUserNameCString[inetUserNameLen] = '\0';
            binaryManip::unicodeToAscii((char*)&internetUserNameValue->data, inetUserNameCString, internetUserNameValue->len);
            internetUserName = inetUserNameCString;
            delete[] inetUserNameCString;
            free(internetUserNameValue);
            internetUserNameValue = NULL;
        }

        samUser *newSamUser;
        try{
            newSamUser = new samUser(rid, userNKOffset, vValue, vValuePath, fValue, fValuePath, this->hashedBootKey, internetUserName);
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

    void samHive::clearPassword(unsigned int userIndex) {
        samUser* user = this->userList.at(userIndex);
        ntreg::keyval* vStructRegValue = user->getVStructRegValue();
        ntreg::user_V *vStruct = (struct ntreg::user_V *)((char*)(&vStructRegValue->data));

        if(!user->getMSAccount().empty()) {
            std::cout << "MS account detected.  Converting to local." << std::endl;
            if(!this->deleteValue(user->getNKOffset(), (char*)"InternetUserName")) {
                throw(new owpException("Failed to delete MS Account value!", 2));
            }

            if(!this->deleteValue(user->getNKOffset(), (char*)"InternetProviderGUID")) {
                throw(new owpException("Failed to delete MS Account value!", 2));
            }

            if(!this->deleteValue(user->getNKOffset(), (char*)"GivenName")) {
                throw(new owpException("Failed to delete MS Account value!", 2));
            }

            if(!this->deleteValue(user->getNKOffset(), (char*)"Surname")) {
                throw(new owpException("Failed to delete MS Account value!", 2));
            }

            if(!this->deleteValue(user->getNKOffset(), (char*)"InternetSID")) {
                throw(new owpException("Failed to delete MS Account value!", 2));
            }

            if(!this->deleteValue(user->getNKOffset(), (char*)"InternetUID")) {
                throw(new owpException("Failed to delete MS Account value!", 2));
            }

            if(!this->deleteValue(user->getNKOffset(), (char*)"ComplexityPolicy")) {
                throw(new owpException("Failed to delete MS Account value!", 2));
            }

            if(!this->deleteValue(user->getNKOffset(), (char*)"ComplexityLastUsed")) {
                throw(new owpException("Failed to delete MS Account value!", 2));
            }

            user->clearMSAccount();
            user->clearFullName();
            vStruct->fullname_len = 0;
        }

        vStruct->lmpw_len = 0;
        vStruct->ntpw_len = 0;
        user->hasChanges(true);
    }

    /**
     * Merges changes made with samUser objects back into the hive in memory
     * @return bool Whether or not all changes were merged
     */
    bool samHive::mergeChangesToHive() {
        bool allSuccessful = true;
        for(unsigned int i = 0; i < userList.size(); i++) {
            if(userList.at(i)->hasChanges()) {
                ntreg::keyval *keyValue = userList.at(i)->getVStructRegValue();
                string path = userList.at(i)->getVStructPath().c_str();
                int size = copyBufferToValue(keyValue, 0, (char*)path.c_str(), VAL_TYPE_REG_BINARY, TPF_VK_EXACT);

                if(size < 1) {
                    allSuccessful = false;
                }
                cout << stringPrintf("Merging into %s: wrote %d bytes", path.c_str(), size) << endl;

                keyValue = userList.at(i)->getFStructRegValue();
                path = userList.at(i)->getFStructPath().c_str();
                size = copyBufferToValue(keyValue, 0, (char*)path.c_str(), VAL_TYPE_REG_BINARY, TPF_VK_EXACT);

                if(size < 1 || allSuccessful == false) {
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

    unsigned char* samHive::getGlobalFValue() {
        unsigned char* fValue;
        fValue = (unsigned char*)ntreg::get_val_data(this->regHive, 0, (char*)"\\SAM\\Domains\\Account\\F", VAL_TYPE_REG_BINARY, 0);

        if(!fValue) {
            throw(new owpException("No valid F Value found in \\SAM\\Domains\\Account"));
        }

        return fValue;
    }

    unsigned char* samHive::generateHashedBootKey(unsigned char* bootKey) {
        unsigned char aqwerty[] = "!@#$%^&*()qwertyUIOPAzxcvbnmQQQQQQQQQQQQ)(*@&%";
        unsigned char anum[] = "0123456789012345678901234567890123456789";

        MD5_CTX md5Context;
        unsigned char md5Hash[0x10];
        RC4_KEY rc4Key;
        unsigned char* hashedBootKey = new unsigned char[0x20];

        unsigned char* fValue = this->getGlobalFValue();

        MD5_Init(&md5Context);
        MD5_Update(&md5Context, &fValue[0x70], 0x10);
        MD5_Update(&md5Context, aqwerty, 0x2f);
        MD5_Update(&md5Context, bootKey, 0x10);
        MD5_Update(&md5Context, anum, 0x29);
        MD5_Final(md5Hash, &md5Context);
        RC4_set_key(&rc4Key, 0x10, md5Hash);
        RC4(&rc4Key, 0x20, &fValue[0x80], hashedBootKey);

        return hashedBootKey;
    }

    samHive::~samHive() {
        if(hashedBootKey) {
            delete[] hashedBootKey;
        }

        for(unsigned int i = 0; i < userList.size(); i++) {
            if(userList.at(i)) {
                delete userList.at(i);
                userList.at(i) = NULL;
            }
        }
        userList.clear();
    }
}
