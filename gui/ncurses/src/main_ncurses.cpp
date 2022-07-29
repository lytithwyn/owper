/*
 * This file is part of owper-gui - a graphical front-end to owper
 * Copyright (C) 2010-2018 Matthew Morgan
 *
 * Some code was borrowed/modified from the chntpw project
 * Copyright (c) 1997-2011 Petter Nordahl-Hagen
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

#include <iostream>
#include <cstring>

#include "include/owper_ncurses.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char* argv[]) {
    std::string hiveFilePath = "";

    if(argc > 1) {
        hiveFilePath = argv[1];
    } else {
        std::cout << "You must specify the path to the directory containing the registry hive files!" << std::endl;
        return 1;
    }

    owper_ncurses *passwordClearer;
    passwordClearer = new owper_ncurses();
    passwordClearer->changeHivePath(hiveFilePath);

    passwordClearer->main();

    delete passwordClearer;

    return 0;
}
