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

#ifndef REG_FILE_HEADER_H
#define REG_FILE_HEADER_H

class regFileHeader {
private:
    long id;            /* 0x00000000 D-Word  ID: ASCII-"regf" = 0x66676572 */
    long unknown1;      /* 0x00000004 D-Word  ???? */
    long unknown2;      /* 0x00000008 D-Word  ???? Always the same value as at 0x00000004  */
    char timestamp[8];  /* 0x0000000C Q-Word  last modify date in WinNT date-format */
    long unknown3;      /* 0x00000014 D-Word  1 */
    long unknown4;      /* 0x00000018 D-Word  3 - probably version #. 2 in NT3.51 */
    long unknown5;      /* 0x0000001C D-Word  0 */
    long unknown6;      /* 0x00000020 D-Word  1 */
    long offsetRootKey;   /* 0x00000024 D-Word  Offset of 1st key record */
    long fileSize;      /* 0x00000028 D-Word  Size of the data-blocks (Filesize-4kb) */
    long unknown7;      /* 0x0000002C D-Word  1 */
    char name[0x1fc-0x2c];   /* Seems like the hive's name is buried here, max len unknown */
    long checksum;      /* 0x000001FC D-Word  Sum of all D-Words from 0x00000000 to 0x000001FB */
public:
    regFileHeader(hive regHive);

    long getChecksum() const{ return checksum; }
    long getFileSize() const { return filesize; }
    long getId() const { return id; }
    char getName() const { return name; }
    long getOffsetRootKey() const { return ofs_rootkey; }
    char getTimestamp() const { return timestamp; }
};

#endif
