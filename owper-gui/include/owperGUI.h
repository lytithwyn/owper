/*
 * This file is part of owper-gui - a graphical front-end to owper
 * Copyright (C) 2010 Matthew Morgan
 *
 * Some code was borrowed/modified from the chntpw project
 * Copyright (c) 1997-2007 Petter Nordahl-Hagen
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
using namespace std;

class owperGUI
{
    private:
        GtkWidget *winMain;
        GtkWidget *vboxMain;
        GtkWidget *frameDirectory;
        GtkWidget *hboxDirectory;
        GtkWidget *entryDirectory;
        GtkWidget *buttonBrowseDirectory;
        GtkWidget *scrollwinUsers;
        GtkWidget *vboxUsers;
        GtkWidget *hboxCommands;
        GtkWidget *buttonClearPasswords;

        string             stringSamFileName;
        string             stringSystemFileName;
        //vector<userWidget*> vectUserWidgets;

    public:
        owperGUI(string stringInitHivePath = "");

        static void delete_event(GtkWidget *widget, GdkEvent  *event, gpointer data);
        static void destroy(GtkWidget *widget, gpointer data);
        static void directory_browse_event(GtkWidget *widget, gpointer data);
        void directory_change(string directory);
        bool directory_test(string directory);
        void clearUsers();
        bool checkAddUser(string stringOutput);
        static void clearPasswords(void);
};


#endif
