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


extern "C" {
    #include "include/ntreg.h"
}

#include "include/owpException.h"

namespace owper {
    typedef int reg_off;

    class hive {
    private:
        string   fileName;         /* Hive's filename */
        int      fileDesc;         /* File descriptor (only valid if state == OPEN) */
        int      state;            /* Current state of hive */
        int      type;             /* Suggested type of hive. NOTE: Library will guess when
                                  it loads it, but application may change it if needed */
        int      pages;            /* Number of pages, total */
        int      usedBlocks;       /* Total # of used blocks */
        int      unusedBlocks;     /* Total # of unused blocks */
        int      usedBytes;        /* total # of bytes in useblk */
        int      unusedBytes;      /* total # of bytes in unuseblk */
        int      size;             /* Hives size (filesise) in bytes */
        reg_off  rootOffset;       /* Offset of root-node */
        short    nkIndexType;      /* Subkey-indextype the root key uses */
        char     *buffer;          /* Files raw contents */

    public:
        hive(const char* fileName, int hiveMode = HMODE_RW);
        ~hive();

        // accessors/modifiers
        string getFileName(){ return this->fileName; };
        int   getType(){ return this->type; };

        reg_off travPath(reg_off startingOffest, char* path, int type);
    };
}

#endif
