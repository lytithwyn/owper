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

#ifndef HIVE_H
#define HIVE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


#include "include/ntreg.h"
#include "include/owpException.h"
#include "include/stringManip.h"

using stringManip::stringPrintf;

namespace owper {
    typedef int reg_off;

    enum HIVE_TYPE {
        UNKNOWN = HTYPE_UNKNOWN,
        SAM = HTYPE_SAM,
        SYSTEM = HTYPE_SYSTEM,
        SECURITY = HTYPE_SECURITY,
        SOFTWARE = HTYPE_SOFTWARE
    };

    enum SCAN_KEY_RESULT {
        ERROR = -1,
        NO_MORE_ITEMS = 0,
        MORE_ITEMS = 1
    };

    class hive {
    private:
        ntreg::hive *regHive;

    public:
        hive(const char* fileName, int hiveMode = HMODE_RW);
        ~hive();

        // accessors/modifiers
        string    getFileName(){ return string(this->regHive->filename); };
        int       getFileDesc(){ return this->regHive->filedesc; };
        int       getState(){ return this->regHive->state; };
        HIVE_TYPE getType(){ return (HIVE_TYPE)this->regHive->type; };
        int       getPages(){ return this->regHive->pages; };
        int       getUsedBlocks(){ return this->regHive->useblk; };
        int       getUnusedBlocks(){ return this->regHive->unuseblk; };
        int       getUsedBytes(){ return this->regHive->usetot; };
        int       getUnusedBytes(){ return this->regHive->unusetot; };
        int       getSize(){ return this->regHive->size; };
        int       getRootOffset(){ return this->regHive->rootofs; };
        short     getNkIndexType(){ return this->regHive->nkindextype; };
        char      *getBuffer(){ return this->regHive->buffer; };

        reg_off travPath(reg_off startingOffest, char* path, int type);
        SCAN_KEY_RESULT getNextSubKey(int nkofs, int *count, int *countri, struct ntreg::ex_data *sptr);

        struct ntreg::keyval *copyValueToBuffer(struct ntreg::keyval *kv, int vofs, char *path, int type);
        int getDword(int vofs, char *path);

        void unicodetoAscii(char *src, char *dest, int l);
        void asciiToUnicode(char *src, char *dest, int l);

    private:
        void openHive(const char* fileName, int hiveMode);
        void closeHive();
    };

}

#endif
