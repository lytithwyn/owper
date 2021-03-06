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

#include "include/owpException.h"

namespace owper {
    owpException::owpException(string message/* = (char*)""*/, int code/* = -9999*/) {
        this->errorMessage = message;
        this->errorCode = code;

        string errorCodeString = "";

        if(code != -9999) {
            errorCodeString  = " [Error#: ";
            errorCodeString += stringManip::intToString(errorCode);
            errorCodeString += "]";
        }

        this->formattedMessage = this->errorMessage + errorCodeString;
    }

    const char* owpException::what() const throw() {
        return this->formattedMessage.c_str();
    }
}
