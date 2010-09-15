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

#ifndef STRING_MANIP_H
#define STRING_MANIP_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdarg>
#include <cstdio>

using std::string;
using std::transform;

namespace stringManip {
    string stringToLower(string inString);
    string intToString(int num);
    string stringPrintf(string formatSpecifier, ...);
}

#endif
