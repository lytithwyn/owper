/*
 * owper - a graphical version of the well known chntpw password changer
 * Copyright (C) 2010 Matthew Morgan
 *
 * Much of this program is modified from chntpw by Petter Nordahl-Hagen
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

#include <iostream>
#include <exception>

#include "include/stringManip.h"

namespace owper{

    class owpException {
    public:
        string errorMessage;
        int errorCode;

        owpException(string message = (char*)"", int code = 0){ this->errorMessage = message; this->errorCode = code; };

        char* what() {
            string errorCodeString = stringManip::intToString(errorCode);

            int messageLength = errorMessage.size() + errorCodeString.size();

            char* outMessage[messageLength];
            snprintf(outMessage, "%s [%s]", messageLength + 4, errorMessage, errorCodeString);

            return outMessage;
        }
    };
}
