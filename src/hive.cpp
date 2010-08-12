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

#include "include/hive.h"
#include "include/owpException.h"

using std::cerr;
using std::endl;

namespace owper {
    hive::hive(const char* fileName, int hiveMode/* = HMODE_RW*/) {
        //unsigned long pageOffset;

        this->fileName = fileName;
        this->state = 0;
        this->size = 0;
        this->buffer = NULL;

        this->openHive(hiveMode);
        this->readHiveToBuffer();
    }

    void hive::openHive(int hiveMode) {
        int fileMode;
        int curHiveMode;

        if(hiveMode & HMODE_RO) {
            fileMode = O_RDONLY;
        } else {
            fileMode = O_RDWR;

        this->fileDesc = open(this->fileName.c_str(), fileMode);

        if(this->fileDesc < 0) {
            if(fileMode == O_RDONLY) {
                throw(owpException(stringPrintf("Could not open hive file: %s", this->fileName.c_str())));
            } else {
                cerr << stringPrintf("Failed to open hive: [\"%s\"] in RW mode, attempting RO", this->fileName.c_str());
                cerr << endl;
                this->fileDesc = open(this->fileName.c_str(), O_RDONLY);
                if(this->fileDesc < 0) {
                    throw(owpException(stringPrintf("Could not open hive file: %s", this->fileName.c_str())));
                }
            }

                cerr << "Opened in RO mode." << endl;
                curHiveMode |= HMODE_RO;
            }
        }

        this->state |= (curHiveMode|HMODE_OPEN);
        cerr << stringPrintf("Opened hive: %s", this->fileName.c_str()) << endl;
    }

    void hive::readHiveToBuffer() {
        struct stat statBuff;

        if ( fstat(this->fileDesc, &statBuff) ) {
            throw(owpException(stringPrintf("Failed to fstat() file: %s", this->fileName.c_str())));
        }

        this->size = statBuff.st_size;

        try {
            this->buffer = new char[this->size];
        }catch(exception e) {
            throw(owpException(stringPrintf("Failed to allocate %d bytes to hold hive: ",
                               this->size, this->fileName.c_str())));
        }

        cerr << stringPrintf("Attempting to read %d bytes from hive: %s", this->size, this->fileName.c_str());
        cerr << endl;
        int bytesRead = read(this->fileDesc, this->buffer, this->size);
        if(bytesRead != this->size) {
            throw owpException(stringPrintf("Failed to read entire hive file.  Expected %d bytes, got %d",
                                            this->size, bytesRead));
        }
    }

    hive::~hive() {
        if(this->state & HMODE_OPEN) {
            cerr << stringPrintf("Closing hive: %s", this->fileName.c_str()) << endl;
            close(this->fileDesc);
        } else {
            cerr << "Not closing hive...not open" << endl;
        }

        if(this->buffer)
            delete[] buffer;
    }

/*  reg_off hive::travPath(reg_off startingOffest, char* path, int type) {

    }*/
}
