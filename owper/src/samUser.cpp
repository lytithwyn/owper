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
#include "include/samUser.h"

namespace owper {
    samUser::samUser(
            int inRid,
            int inNKOffset,
            ntreg::keyval *inVStructRegValue,
            string inVStructPath,
            ntreg::keyval *inFStructRegValue,
            string inFStructPath,
            unsigned char* inHashedBootKey,
            string msAccount/* = ""*/
    ) {
        rid = inRid;
        nkOffset = inNKOffset;
        vStructPath = inVStructPath;
        vStructRegValue = inVStructRegValue;
        fStructPath = inFStructPath;
        fStructRegValue = inFStructRegValue;
        keySched1 = NULL;
        keySched2 = NULL;

        if(inHashedBootKey) {
            hashedBootKey = new unsigned char[0x20];
            memcpy(hashedBootKey, inHashedBootKey, 0x20);
        } else {
            hashedBootKey = 0;
        }

        vStruct = (struct ntreg::user_V *)((char*)(&inVStructRegValue->data));
        char* vBuffer = (char*)&(inVStructRegValue->data);

        fStruct = (struct ntreg::user_F *)((char*)(&inFStructRegValue->data));

        int userNameOffset = vStruct->username_ofs;
        int userNameLength = vStruct->username_len;
        int fullNameOffset = vStruct->fullname_ofs;
        int fullNameLength = vStruct->fullname_len;
        int vStructLength = inVStructRegValue->len;

        if(!hasValidUserName(userNameOffset, userNameLength, vStructLength)) {
            FREE(fStruct);
            FREE(vStruct);
            throw(new owpException("VStruct has invalid user name field"));
        }

        if(!hasValidFullName(fullNameOffset, fullNameLength, vStructLength)) {
            FREE(fStruct);
            FREE(vStruct);
            throw(new owpException("VStruct has invalid full name field"));
        }

        userName = this->getUserValue(vBuffer, userNameOffset, userNameLength);
        fullName = this->getUserValue(vBuffer, fullNameOffset, fullNameLength);
        this->msAccount = msAccount;

        if(hashedBootKey) {
            lmHash = this->decryptHash(vStruct->lmpw_ofs, vStruct->lmpw_len, "LMPASSWORD");
            ntHash = this->decryptHash(vStruct->ntpw_ofs, vStruct->ntpw_len, "NTPASSWORD");
        }

        regDataChanged = false;
    }

    unsigned char* samUser::decryptHash(int hashOffset, int hashLength, const char* extraHashInput) {
        if(!hashedBootKey) {
            std::cerr << "No hashed boot key available!\n\n";
            return NULL;
        }

        if(hashLength != 0x14) {
            return NULL;
        }

        MD5_CTX md5Context;
        unsigned char md5Hash[0x10];
        unsigned char obfKey[0x10];
        RC4_KEY rc4Key;
        hashOffset += 0xCC;

        MD5_Init(&md5Context);
        MD5_Update(&md5Context, hashedBootKey, 0x10);
        MD5_Update(&md5Context, &rid, 0x4);
        MD5_Update(&md5Context, extraHashInput, 0xb);
        MD5_Final(md5Hash, &md5Context);
        RC4_set_key( &rc4Key, 0x10, md5Hash);
        RC4( &rc4Key, 0x10, (const unsigned char*)vBuffer + hashOffset + 4, obfKey);

        //these keys are cached as member variables
        if(keySched1 == 0 || keySched2 == 0) {
            /* Get the two decrypt keys. */
            keySched1 = new des_key_schedule;
            keySched2 = new des_key_schedule;
            des_cblock desKey1, desKey2;

            ridToKey1(rid,(unsigned char *)desKey1);
            des_set_key_checked((des_cblock *)desKey1, (*keySched1));
            ridToKey2(rid,(unsigned char *)desKey2);
            des_set_key_unchecked((des_cblock *)desKey2, (*keySched2));
        }

        unsigned char *decryptedHash = new unsigned char[0x10];

        /* Decrypt the password hash as two 8 byte blocks. */
        des_ecb_encrypt((des_cblock *)obfKey,
                (des_cblock *)decryptedHash, (*keySched1), DES_DECRYPT);
        des_ecb_encrypt((des_cblock *)(obfKey + 8),
                (des_cblock *)&decryptedHash[8], (*keySched2), DES_DECRYPT);

        return decryptedHash;
    }

    bool samUser::hashIsEmpty(unsigned const char* hash, const char* emptyHashPreset) {
        if(!hash) {
            //the hash contains nothing
            return true;
        }

        if(!emptyHashPreset) {
            //there is no "empty" has preset to compare to - report non-empty hash
            return false;
        }

        try {
            if(compareHash(hash, emptyHashPreset) == 0) {
                //the hash is the same as the "empty" preset - report empty hash
                return true;
            }
        } catch(owpException *exception) {
            //we thought there was data in that array...guess not...report empty hash
            std::cerr << "This should NEVER happen...we checked that the hash had contents, but compareHash says it's empty." << std::endl;
            delete exception;
            return true;
        }

        return false;
    }

    char* samUser::hashToString(unsigned const char* hash) {
        char *hashChars;

        if(!hash) {
            hashChars = new char[1];
            hashChars[0] = '\0';
            return hashChars;
        }

        //convert the hash to a string
        hashChars = new char[33];
        for(int i = 0; i < 0x10 ; i++) {
            sprintf(&(hashChars[i*2]), "%.2x", hash[i]);
        }

        hashChars[32] = '\0';
        return hashChars;
    }

    int samUser::compareHash(unsigned const char* hash, const char* stringToCompare) {
        if(!hash) {
            throw(new owpException("Invalid hash specified for comparison!"));
        }

        if(!stringToCompare) {
            throw(new owpException("Invalid string specified for comparison!"));
        }

        char *hashChars = hashToString(hash);

        return strcmp(hashChars, stringToCompare);
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

    bool samUser::passwordIsBlank() {
        bool hasBlankPassword = true;

        if(!this->msAccount.empty()) {
            hasBlankPassword = false;
        } else {
            if(this->hashedBootKey) {
                if(!hashIsEmpty(lmHash, "aad3b435b51404eeaad3b435b51404ee")) {
                    hasBlankPassword = false;
                }

                if(!hashIsEmpty(ntHash, "31d6cfe0d16ae931b73c59d7e0c089c0")) {
                    hasBlankPassword = false;
                }
            } else {
                if(vStruct->lmpw_len == 0x14 || vStruct->lmpw_len == 0x14) {
                    hasBlankPassword = false;
                }
            }
        }

        return hasBlankPassword;
    }

    bool samUser::accountIsDisabled() const {
        std::cout << "RID " << fStruct->rid << " ACB_bits: 0x" << std::hex << fStruct->ACB_bits << std::endl;
        return (fStruct->ACB_bits & ACB_DISABLED || fStruct->ACB_bits & ACB_AUTOLOCK)?(true):(false);
    }

    void samUser::enableAccount() {
        if(!accountIsDisabled()) {
            return;
        }

        ntreg::uint16_t acb = fStruct->ACB_bits;
        acb |= ACB_PWNOEXP;
        acb &= ~ACB_DISABLED;
        acb &= ~ACB_AUTOLOCK;
        fStruct->ACB_bits = acb;
        fStruct->failedcnt = 0;
        regDataChanged = true;
    }

    void samUser::disableAccount() {
        if(accountIsDisabled()) {
            return;
        }

        ntreg::uint16_t acb = fStruct->ACB_bits;
        acb |= ACB_DISABLED;
        fStruct->ACB_bits = acb;
        regDataChanged = true;
    }

    void samUser::strToDesKey(unsigned char *str,unsigned char *key)
    {
        int i;

        key[0] = str[0]>>1;
        key[1] = ((str[0]&0x01)<<6) | (str[1]>>2);
        key[2] = ((str[1]&0x03)<<5) | (str[2]>>3);
        key[3] = ((str[2]&0x07)<<4) | (str[3]>>4);
        key[4] = ((str[3]&0x0F)<<3) | (str[4]>>5);
        key[5] = ((str[4]&0x1F)<<2) | (str[5]>>6);
        key[6] = ((str[5]&0x3F)<<1) | (str[6]>>7);
        key[7] = str[6]&0x7F;
        for (i=0;i<8;i++) {
            key[i] = (key[i]<<1);
        }
        des_set_odd_parity((des_cblock *)key);
    }

    void samUser::ridToKey1(unsigned long rid,unsigned char deskey[8])
    {
        unsigned char s[7];

        s[0] = (unsigned char)(rid & 0xFF);
        s[1] = (unsigned char)((rid>>8) & 0xFF);
        s[2] = (unsigned char)((rid>>16) & 0xFF);
        s[3] = (unsigned char)((rid>>24) & 0xFF);
        s[4] = s[0];
        s[5] = s[1];
        s[6] = s[2];

        strToDesKey(s,deskey);
    }

    void samUser::ridToKey2(unsigned long rid,unsigned char deskey[8])
    {
        unsigned char s[7];

        s[0] = (unsigned char)((rid>>24) & 0xFF);
        s[1] = (unsigned char)(rid & 0xFF);
        s[2] = (unsigned char)((rid>>8) & 0xFF);
        s[3] = (unsigned char)((rid>>16) & 0xFF);
        s[4] = s[0];
        s[5] = s[1];
        s[6] = s[2];

        strToDesKey(s,deskey);
    }

    samUser::~samUser() {
        FREE(vStructRegValue);
        FREE(fStructRegValue);

        if(hashedBootKey) { delete[] hashedBootKey; };
        if(lmHash) { delete[] lmHash; };
        if(ntHash) { delete[] ntHash; };
        if(keySched1) { delete keySched1; };
        if(keySched2) { delete keySched2; };
    }
}
