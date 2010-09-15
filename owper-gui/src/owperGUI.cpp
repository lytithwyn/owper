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
#include "include/owperGUI.h"

owperGUI::owperGUI( string initHivePath/*=""*/) {
    winMain = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width(GTK_CONTAINER(winMain), 5);
    gtk_window_set_default_size(GTK_WINDOW(winMain), 600, 400);
    g_signal_connect (G_OBJECT(winMain), "delete_event", G_CALLBACK(delete_event), NULL);
    g_signal_connect (G_OBJECT (winMain), "destroy", G_CALLBACK (destroy), NULL);

    vboxMain = gtk_vbox_new(false, 5);


    //section for setting the directory from which to retrieve registry files
    frameDirectory = gtk_frame_new("Select Regsitry Files Directory");
    gtk_box_pack_start(GTK_BOX(vboxMain), frameDirectory, false, false, 0);

    hboxDirectory = gtk_hbox_new(false, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hboxDirectory), 5);
    gtk_container_add(GTK_CONTAINER(frameDirectory), hboxDirectory);

    entryDirectory = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entryDirectory), 30);
    gtk_entry_set_editable(GTK_ENTRY(entryDirectory), false);
    gtk_box_pack_start(GTK_BOX(hboxDirectory), entryDirectory, true, true, 5);

    buttonBrowseDirectory = gtk_button_new_with_mnemonic("_Browse");
    /*g_signal_connect (G_OBJECT(buttonBrowseDirectory), "clicked",
        G_CALLBACK(directory_browse_event), NULL);*/
    gtk_box_pack_start(GTK_BOX(hboxDirectory), buttonBrowseDirectory, false, false, 5);


    //section for list of users
    scrollwinUsers = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vboxMain), scrollwinUsers, true, true, 5);

    vboxUsers = gtk_vbox_new(false, 0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollwinUsers), vboxUsers);


    //commands across the bottom
    hboxCommands = gtk_hbox_new(false, 0);
    gtk_box_pack_start(GTK_BOX(vboxMain), hboxCommands, false, false, 0);

    buttonClearPasswords = gtk_button_new_with_mnemonic("_Clear Passwords");
    /*gtk_signal_connect (GTK_OBJECT (buttonClearPasswords), "clicked",
        GTK_SIGNAL_FUNC (clearPasswords), NULL);*/
    gtk_box_pack_end(GTK_BOX(hboxCommands), buttonClearPasswords, false, false, 0);


    gtk_container_add(GTK_CONTAINER (winMain), vboxMain);

    gtk_widget_show_all(winMain);
}

void owperGUI::delete_event(GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    return gtk_main_quit ();
}

void owperGUI::destroy(GtkWidget *widget, gpointer data )
{
    gtk_main_quit ();
}

