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
        HIVE_TYPE_UNKNOWN = HTYPE_UNKNOWN,
        HIVE_TYPE_SAM = HTYPE_SAM,
        HIVE_TYPE_SYSTEM = HTYPE_SYSTEM,
        HIVE_TYPE_SECURITY = HTYPE_SECURITY,
        HIVE_TYPE_SOFTWARE = HTYPE_SOFTWARE
    };

    enum SCAN_KEY_RESULT {
        SCAN_KEY_ERROR = -1,
        SCAN_KEY_NO_MORE_ITEMS = 0,
        SCAN_KEY_MORE_ITEMS = 1
    };

    enum REG_VALUE_TYPE {
        VAL_TYPE_REG_NONE = REG_NONE,
        VAL_TYPE_REG_SZ = REG_SZ,
        VAL_TYPE_REG_EXPAND_SZ = REG_EXPAND_SZ,
        VAL_TYPE_REG_BINARY = REG_BINARY,
        VAL_TYPE_REG_DWORD = REG_DWORD,
        VAL_TYPE_REG_DWORD_BIG_ENDIAN = REG_DWORD_BIG_ENDIAN,
        VAL_TYPE_REG_LINK = REG_LINK,
        VAL_TYPE_REG_MULTI_SZ = REG_MULTI_SZ,
        VAL_TYPE_REG_RESOURCE_LIST = REG_RESOURCE_LIST,
        VAL_TYPE_REG_FULL_RESOURCE_DESCRIPTOR  =  REG_FULL_RESOURCE_DESCRIPTOR,
        VAL_TYPE_REG_RESOURCE_REQUIREMENTS_LIST = REG_RESOURCE_REQUIREMENTS_LIST,
    };

    class hive {
    protected:
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

        struct ntreg::keyval *copyValueToBuffer(struct ntreg::keyval *kv, int vofs, char *path, int type, int exact = 0);
        int copyBufferToValue(struct ntreg::keyval *regValue, int valueOffset, char *path, REG_VALUE_TYPE type, int exact = 0);
        int getDword(int vofs, char *path, int exact = 0);
        bool writeHiveToFile();

    private:
        void openHive(const char* fileName, int hiveMode);

    protected:
        void closeHive();
    };

}

#endif
