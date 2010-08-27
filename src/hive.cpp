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

using std::cout;
using std::cerr;
using std::endl;

namespace owper {
    hive::hive(const char* fileName, int hiveMode/* = HMODE_RW*/) {
        this->openHive(fileName, hiveMode);
    }

    void hive::openHive(const char* fileName, int hiveMode) {
        if((this->regHive = ntreg::openHive((char *)fileName, hiveMode)) == NULL) {
            //ntreg::openHive automatically calls closeHive on failure
            throw(owpException(stringPrintf("Failed to open/read hive file: %s", fileName)));
        }
    }

    void hive::closeHive() {
        ntreg::closeHive(this->regHive);
    }

    hive::~hive() {
        closeHive();
    }

/*  reg_off hive::travPath(reg_off startingOffest, char* path, int type) {

    }*/
}
