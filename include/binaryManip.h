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

#ifndef BINARY_MANIP_H
#define BINARY_MANIP_H

namespace binaryManip {
    /*
     * This class provides a way to specify a number of bytes to copy
     * from an binary file and a pointer to an address to copy them to.
     * The caller is responsible for correct size and handling the memory.
     */
    class field {
    public:
        const void* fieldData;
        const int fieldSize;

    public:
        /*
         * @param A pointer to the memory area that will hold the copied data; If NULL, the data will be thrown away
         * @param int fieldSize The number of bytes to copy or throw away
         */
        field(void* inFieldData, int inFieldSize){ fieldData = inFieldData ; fieldSize = inFieldSize; };
    };

    /*
     * Copy data specified by the fields in outputFieldArray in order
     * @param char* inputBuffer A pointer to the buffer we want to copy from
     * @param field* An array of field objects which specify the data to be copied
     * @param int initialOffset An optional offset to skip to before reading the first field
     */
    void copyFields(char* inputBuffer, field* outputFieldArray, int initialOffset = 0);
}

#endif
