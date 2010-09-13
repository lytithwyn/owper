/*
 * This file is part of owper - a graphical version of the well known chntpw password changer
 * Copyright (C) 2010 Matthew Morgan
 *
 * Some code was borrowed/modified from the chntpw project
 * Copyright (c) 1997-2007 Petter Nordahl-Hagen
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * See file LGPL.txt for the full license.
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
