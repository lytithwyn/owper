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
#include "include/owper_gtk.h"

owper_gtk::owper_gtk() : baseOwperGUI() {
    this->loadGUI();
}

void owper_gtk::loadGUI() {
    winMain = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(winMain), baseOwperGUI::WIN_TITLE);
    gtk_container_set_border_width(GTK_CONTAINER(winMain), 5);
    gtk_window_set_default_size(GTK_WINDOW(winMain), 600, 400);
    g_signal_connect (G_OBJECT(winMain), "delete_event", G_CALLBACK(delete_event), NULL);
    g_signal_connect (G_OBJECT (winMain), "destroy", G_CALLBACK (destroy), NULL);

    vboxMain = gtk_vbox_new(false, 5);


    //section for setting the directory from which to retrieve registry files
    frameHivePath = gtk_frame_new("Registry File Directory");
    gtk_box_pack_start(GTK_BOX(vboxMain), frameHivePath, false, false, 0);

    hboxHivePath = gtk_hbox_new(false, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hboxHivePath), 5);
    gtk_container_add(GTK_CONTAINER(frameHivePath), hboxHivePath);

    entryHivePath = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entryHivePath), 30);
    gtk_editable_set_editable(GTK_EDITABLE(entryHivePath), false);
    gtk_box_pack_start(GTK_BOX(hboxHivePath), entryHivePath, true, true, 5);

    buttonBrowseHivePath = gtk_button_new_with_mnemonic("_Browse");
    g_signal_connect (G_OBJECT(buttonBrowseHivePath), "clicked",
        G_CALLBACK(hive_path_browse_event), (gpointer)this);
    gtk_box_pack_start(GTK_BOX(hboxHivePath), buttonBrowseHivePath, false, false, 5);


    //section for list of users
    scrollwinUsers = gtk_scrolled_window_new(NULL, NULL);
    gtk_box_pack_start(GTK_BOX(vboxMain), scrollwinUsers, true, true, 5);

    vboxUsers = gtk_vbox_new(false, 0);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrollwinUsers), vboxUsers);

    //commands across the bottom
    hboxCommands = gtk_hbox_new(false, 0);
    gtk_box_pack_start(GTK_BOX(vboxMain), hboxCommands, false, false, 0);

    buttonClearPasswords = gtk_button_new_with_mnemonic("_Clear Passwords");
    g_signal_connect (G_OBJECT (buttonClearPasswords), "clicked",
            G_CALLBACK(handleClearPasswords), (gpointer)this);
    gtk_box_pack_end(GTK_BOX(hboxCommands), buttonClearPasswords, false, false, 0);

    buttonEnableAccounts = gtk_button_new_with_mnemonic("_Enable Accounts");
    g_signal_connect (G_OBJECT (buttonEnableAccounts), "clicked",
            G_CALLBACK(handleEnableAccounts), (gpointer)this);
    gtk_box_pack_end(GTK_BOX(hboxCommands), buttonEnableAccounts, false, false, 0);

    buttonDisableAccounts = gtk_button_new_with_mnemonic("_Disable Accounts");
    g_signal_connect (G_OBJECT (buttonDisableAccounts), "clicked",
            G_CALLBACK(handleDisableAccounts), (gpointer)this);
    gtk_box_pack_end(GTK_BOX(hboxCommands), buttonDisableAccounts, false, false, 0);

    gtk_container_add(GTK_CONTAINER (winMain), vboxMain);

    gtk_widget_show_all(winMain);
}

void owper_gtk::delete_event(GtkWidget *widget, GdkEvent  *event, gpointer data)
{
    return gtk_main_quit ();
}

void owper_gtk::destroy(GtkWidget *widget, gpointer data )
{
    gtk_main_quit ();
}

void owper_gtk::hive_path_browse_event(GtkWidget *widget, gpointer owperGUIInstance)
{
    owper_gtk *thisOwperGUI = (owper_gtk*)owperGUIInstance;

    GtkWidget *fileChooser = gtk_file_chooser_dialog_new ("Select directory containing registry files",
                                GTK_WINDOW(thisOwperGUI->winMain),
                                GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                                GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(fileChooser)) == GTK_RESPONSE_ACCEPT)
    {
        char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (fileChooser));
        thisOwperGUI->changeHivePath(filename);
        g_free(filename);
    }

    gtk_widget_destroy (fileChooser);
}

HIVE_LOAD_RESULT owper_gtk::changeHivePath(string newPath) {
    gtk_entry_set_text(GTK_ENTRY(this->entryHivePath), "");
    HIVE_LOAD_RESULT loadResult = 0;

    try {
        loadResult = baseOwperGUI::changeHivePath(newPath); // will only throw an exception if we fail to load the SAM
        gtk_entry_set_text(GTK_ENTRY(entryHivePath), newPath.c_str());
        if(!(loadResult & HIVE_LOAD_HAS_DEFAULT)) {
            GtkWidget *errorDialog = gtk_message_dialog_new (GTK_WINDOW(this->winMain),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "%s",
                                             baseOwperGUI::WARN_DFLT_HIVE_MISSING);
            gtk_dialog_run (GTK_DIALOG (errorDialog));
            gtk_widget_destroy (errorDialog);
        }
    } catch (owpException *exception) {
        GtkWidget *errorDialog = gtk_message_dialog_new (GTK_WINDOW(this->winMain),
                                         GTK_DIALOG_DESTROY_WITH_PARENT,
                                         GTK_MESSAGE_ERROR,
                                         GTK_BUTTONS_CLOSE,
                                         "Error loading hive:\n%s",
                                         exception->errorMessage.c_str());
        gtk_dialog_run (GTK_DIALOG (errorDialog));
        gtk_widget_destroy (errorDialog);
    }

    return loadResult;
}

void owper_gtk::loadUsers() {
    vector<samUser*> samUserList = sam->getUserList();

    for(unsigned int i = 0; i < samUserList.size(); i++) {
        userWidget *curUserWidget = new userWidget(samUserList.at(i), i);

        if(this->deflt == NULL && !(samUserList.at(i)->getMSAccount().empty())) {
            gtk_widget_set_sensitive(curUserWidget->getWidget(), 0);
        }

        vectUserWidgets.push_back(curUserWidget);
        gtk_box_pack_start(GTK_BOX(this->vboxUsers), curUserWidget->getWidget(), false, false, 0);
        gtk_widget_show(curUserWidget->getWidget());
    }
}

void owper_gtk::clearUsers(bool isShutdown/* = false */) {
    // When the close button is clicked on the window, gtk starts destroying
    // all the widgets and child widgets.
    // This will cause a double free if we manually destroy the user widgets
    // during that process, so we set "shutdown" to true when calling from the
    // main window's destructor
    if(!isShutdown) {
        for(unsigned int i = 0; i < vectUserWidgets.size(); i++) {
            vectUserWidgets.at(i)->destroyWidget();
        }
    }

    baseOwperGUI::clearUsers(isShutdown);
}

void owper_gtk::handleClearPasswords(GtkWidget *widget, gpointer owperGUIInstance) {
    owper_gtk *thisOwperGUI = (owper_gtk*)owperGUIInstance;
    thisOwperGUI->clearPasswords();
}

void owper_gtk::handleEnableAccounts(GtkWidget *widget, gpointer owperGUIInstance) {
    owper_gtk *thisOwperGUI = (owper_gtk*)owperGUIInstance;
    thisOwperGUI->enableAccounts();
}

void owper_gtk::handleDisableAccounts(GtkWidget *widget, gpointer owperGUIInstance) {
    owper_gtk *thisOwperGUI = (owper_gtk*)owperGUIInstance;
    thisOwperGUI->disableAccounts();
}

void owper_gtk::reportMergeFailure() {
    GtkWidget *errorDialog = gtk_message_dialog_new (GTK_WINDOW(this->winMain),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "%s",
                                             baseOwperGUI::ERR_MSG_MERGE_FAIL);
    gtk_dialog_run (GTK_DIALOG (errorDialog));
    gtk_widget_destroy (errorDialog);
    return;
}

void owper_gtk::reportSaveFailure(){
    GtkWidget *errorDialog = gtk_message_dialog_new (GTK_WINDOW(this->winMain),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "%s",
                                             baseOwperGUI::ERR_MSG_SAVE_FAIL);
    gtk_dialog_run (GTK_DIALOG (errorDialog));
    gtk_widget_destroy (errorDialog);
    return;
}

void owper_gtk::reportSuccess(string taskDesc) {
    GtkWidget *infoDialog = gtk_message_dialog_new (GTK_WINDOW(this->winMain),
                                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                                     GTK_MESSAGE_INFO,
                                                     GTK_BUTTONS_CLOSE,
                                                     "%s",
                                                     taskDesc.c_str());
    gtk_dialog_run (GTK_DIALOG (infoDialog));
    gtk_widget_destroy (infoDialog);
}

owper_gtk::~owper_gtk() {
    this->clearUsers(true);

}

