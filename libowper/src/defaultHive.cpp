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

#include "include/defaultHive.h"

namespace owper {
    defaultHive::defaultHive(const char* fileName, int hiveMode/* = HMODE_RO*/):
            hive(fileName, hiveMode) {
        if(this->getType() != HIVE_TYPE_USER) {
            this->closeHive();
            throw(new owpException("The filename given does not point to a USER type hive"));
        }

        this->identityStoreNKOffset = this->travPath(0, (char*)"Software\\Microsoft\\IdentityCRL\\StoredIdentities\\", 0) + 4;
        if(!identityStoreNKOffset) {
            this->closeHive();
            throw(new owpException("Could not find the StoredIdentities key in DEFAULT!"));
        }
    }

    void defaultHive::deleteStoredIdentity(std::string identityName) {
        this->deleteKey(this->identityStoreNKOffset, (char*)identityName.c_str());
    }
}

