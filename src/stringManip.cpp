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

#include "include/stringManip.h"


namespace stringManip {
    /**
     * Take a string, and return a copy of it all lower case
     * @param string The string to convert to lower case
     * @return string The string in all lower case
     */
    string stringToLower(string inString) {
        string stringLower = inString; // to fix freakiness in transform
        transform(inString.begin(), inString.end(), stringLower.begin(),
            static_cast < int(*)(int) > (tolower));

        return stringLower;
    }

    /**
     * Take an int and convert it to a string
     * @param int to convert
     * @return string
     */
    string intToString(int num)
    {
        char *castingString;
        int len;

        len = snprintf(NULL, 0, "%d", num);

        if(!(castingString = (char *)calloc(1, (len + 1) * sizeof(char))))
                return string((char *)"");

        snprintf(castingString, len + 1, "%d", num);

        return string(castingString);
    }
}
