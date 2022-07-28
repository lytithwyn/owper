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
#ifndef OWPER_GUI_H
#define OWPER_GUI_H

#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <vector>

#include "include/samHive.h"
#include "include/systemHive.h"
#include "include/defaultHive.h"
#include "include/userWidget.h"
#include "include/fileManip.h"
#include "include/baseOwperGUI.h" // from ../../gui/base

using namespace std;
using namespace owper;
using namespace fileManip;

class owper_gtk : public baseOwperGUI
{
    private:
        GtkWidget *winMain;
        GtkWidget *vboxMain;
        GtkWidget *frameHivePath;
        GtkWidget *hboxHivePath;
        GtkWidget *entryHivePath;
        GtkWidget *buttonBrowseHivePath;
        GtkWidget *scrollwinUsers;
        GtkWidget *vboxUsers;
        GtkWidget *hboxCommands;
        GtkWidget *buttonClearPasswords;
        GtkWidget *buttonEnableAccounts;
        GtkWidget *buttonDisableAccounts;

    protected:
        void loadGUI();

    public:
        owper_gtk();
        ~owper_gtk();

        static void delete_event(GtkWidget *widget, GdkEvent  *event, gpointer data);
        static void destroy(GtkWidget *widget, gpointer data);
        static void hive_path_browse_event(GtkWidget *widget, gpointer owperGUIInstance);
        HIVE_LOAD_RESULT changeHivePath(string newPath);
        void clearUsers(bool isShutdown = false);
        void loadUsers();
        static void handleClearPasswords(GtkWidget *widget, gpointer owperGUIInstance);
        static void handleEnableAccounts(GtkWidget *widget, gpointer owperGUIInstance);
        static void handleDisableAccounts(GtkWidget *widget, gpointer owperGUIInstance);
        void reportSuccess(string successMessage);
        void reportMergeFailure();
        void reportSaveFailure();
};


#endif
