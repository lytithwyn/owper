/*
 * This file is part of owper - a c++ windows password changing library based on chntpw
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

#ifndef OWP_EXCEPTION_H
#define OWP_EXCEPTION_H

#include <iostream>
#include <exception>

#include "include/stringManip.h"

using std::exception;

namespace owper{

    class owpException : public exception {
    public:
        string errorMessage;
        int errorCode;
        string formattedMessage;

        owpException(string message = (char*)"", int code = -9999);
        virtual ~owpException() throw(){ /* we have no heap variables--nothing to do */ };
        virtual const char* what() const throw();
    };
}

#endif
