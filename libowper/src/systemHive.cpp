/*
 * This file is part of owper - a c++ windows password changing library based on chntpw
 * Copyright (C) 2010-2011 Matthew Morgan
 *
 * Some code was borrowed/modified from the following projects:
 *
 * chntpw   - Copyright (c) 1997-2007 Petter Nordahl-Hagen
 * bkhive   - Copyright (C) 2004-2006 Nicola Cuomo <ncuomo@studenti.unina.it>
 * samdump2 - Copyright (C) 2004-2006 Nicola Cuomo <ncuomo@studenti.unina.it>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2 of the License.
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

#if BYTE_ORDER == LITTLE_ENDIAN
  int bootKeyPermOrder[] = { 0xb, 0x6, 0x7, 0x1, 0x8, 0xa, 0xe, 0x0, 0x3, 0x5, 0x2,
                             0xf, 0xd, 0x9, 0xc, 0x4 };
#elif BYTE_ORDER == BIG_ENDIAN
  int bootKeyPermOrder[] = { 0x8, 0x5, 0x4, 0x2, 0xb, 0x9, 0xd, 0x3, 0x0, 0x6, 0x1,
                             0xc, 0xe, 0xa, 0xf, 0x7 };
#endif

#include "include/systemHive.h"

namespace owper {
    systemHive::systemHive(const char* fileName, int hiveMode/* = HMODE_RO*/):
            hive(fileName, hiveMode) {
        if(this->getType() != HIVE_TYPE_SYSTEM) {
            this->closeHive();
            throw(new owpException("The filename given does not point to a SYSTEM type hive"));
        }
    }

    int systemHive::getDefaultControlSet() {
        return ntreg::get_dword(this->regHive, 0, (char*)"\\Select\\Default", 0);
    }

    unsigned char* systemHive::getBootKey() {
        int currentControlSet = getDefaultControlSet();
        char *keyNames[] = {(char*)"JD", (char*)"Skew1", (char*)"GBG", (char*)"Data"};

        string curKeyPath;
        char* curClassName;
        unsigned char unsortedBootKey[0x10] = {0};
        for(int keyIndex = 0; keyIndex < 4; keyIndex++) {
            curKeyPath = stringPrintf("\\ControlSet00%d\\Control\\Lsa\\%s",
                                      currentControlSet,
                                      keyNames[keyIndex]
                         );

            try {
                curClassName = this->getClassName((char*)curKeyPath.c_str());
            } catch(owpException *exception) {
                //this path is no good - re-throw
                throw(exception);
            }

            sscanf(curClassName, "%x", (int*)(&unsortedBootKey[keyIndex*4]));

            delete[] curClassName;
            curClassName = 0;
        }

        unsigned char *sortedBootKey = new unsigned char[0x10];
        this->sortBootKey(unsortedBootKey, sortedBootKey);

        return sortedBootKey;
    }

    void systemHive::sortBootKey(unsigned char* unsortedBootKey, unsigned char* sortedBootKey) {
        for(int i = 0; i < 0x10; i++)  {
            sortedBootKey[i] = unsortedBootKey[bootKeyPermOrder[i]];
        }
    }

    char* systemHive::getClassName(char* nkKeyPath) {
        reg_off nkKeyOffset = this->travPath(0, nkKeyPath, 0);
        struct ntreg::nk_key *nkKey;

        try {
            nkKey = this->getNkKeyAtOffset(nkKeyOffset);
        }catch(owpException *exception) {
            //we must have supplied a bad path
            delete exception;
            throw(new owpException(stringPrintf("Invalid nk_key path: %s", nkKeyPath)));
        }

        char *className = new char[9];
        int shortestLength = (nkKey->len_classnam > (8 * 2))?(8 * 2):(nkKey->len_classnam);

        int classNameOffset= nkKey->ofs_classnam + 4;
        classNameOffset += 0x1000; //this additional offset was taken from bkhive.c:136 (v1.1.1)

        ntreg::cheap_uni2ascii((char*)(this->regHive->buffer + classNameOffset), className, shortestLength);
        return className;
    }
}
