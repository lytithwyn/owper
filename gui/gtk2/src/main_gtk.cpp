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

#include "include/owperGUI.h"

using std::cout;
using std::endl;
using std::string;
using stringManip::stringPrintf;

int main(int argc, char* argv[]) {
    gtk_init(&argc, &argv);

    std::string hiveFilePath = "";

    if(argc > 1) {
        hiveFilePath = argv[1];
    }

    owperGUI *passwordClearer;
    passwordClearer = new owperGUI(hiveFilePath);

    gtk_main();

    delete passwordClearer;

    return 0;

}
