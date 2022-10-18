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
#ifndef OWPER_NCURSES_GUI_H
#define OWPER_NCURSES_GUI_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>

// ncurses stuff
#include <menu.h>
#include <panel.h>

#include "include/userWidget.h"
#include "include/baseOwperGUI.h" // from ../../gui/base

using namespace std;
using namespace owper;

class owper_ncurses : public baseOwperGUI
{
    private:
    ITEM**  userItems;
    WINDOW* owperMenuWin;
    WINDOW* owperMenuSubWin;
    MENU*   owperMenu;
    SCREEN* owperScreen;
    HIVE_LOAD_RESULT hiveLoadResult;

    void reloadUsers();

    protected:
        void loadGUI();

    public:
        owper_ncurses();
        ~owper_ncurses();

        void main(); // actually start up the gui and do the things

        HIVE_LOAD_RESULT changeHivePath(string newPath);
        void clearUsers(bool isShutdown = false);
        void loadUsers();
        void reportSuccess(string successMessage) { this->displayMessage("", successMessage.c_str()); };
        void reportMergeFailure() { this->displayMessage("Error", baseOwperGUI::ERR_MSG_MERGE_FAIL); };
        void reportSaveFailure() { this->displayMessage("Error", baseOwperGUI::ERR_MSG_SAVE_FAIL); };
        void displayMessage(const char* title, const char* message); // can't be called until after loadGUI()
        void refreshMainGUI();
};


#endif
