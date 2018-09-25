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

#ifndef SYSTEM_HIVE_H
#define SYSTEM_HIVE_H

#include <iostream>
#include <vector>
#include "include/hive.h"
#include "include/stringManip.h"

using std::vector;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

namespace owper {
    class systemHive : public hive {
    private:
        int getDefaultControlSet();
        void sortBootKey(unsigned char* unsortedBootKey, unsigned char* sortedBootKey);
        char* getClassName(char* nkKeyPath);
    public:
        systemHive(const char* fileName, int hiveMode = HMODE_RO);

        unsigned char* getBootKey();
    };

}

#endif
