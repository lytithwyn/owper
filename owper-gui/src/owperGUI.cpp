/*
 * This file is part of owper-gui - a graphical front-end to owper
 * Copyright (C) 2010-2013 Matthew Morgan
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
#include "include/owperGUI.h"

owperGUI::owperGUI(string initHivePath/*=""*/, samHive* preloadedSam/*=NULL*/) {
    sam = NULL;
    loadGUI();

    if(!initHivePath.empty()) {
        changeHiveFile(initHivePath, preloadedSam);
    }
}

void owperGUI::loadGUI() {
    winMain = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_container_set_border_width(GTK_CONTAINER(winMain), 5);
    gtk_window_set_default_size(GTK_WINDOW(winMain), 600, 400);
    g_signal_connect (G_OBJECT(winMain), "delete_event", G_CALLBACK(delete_event), NULL);
    g_signal_connect (G_OBJECT (winMain), "destroy", G_CALLBACK (destroy), NULL);

    vboxMain = gtk_vbox_new(false, 5);


    //section for setting the directory from which to retrieve registry files
    frameSamFile = gtk_frame_new("Select Sam Registry File");
    gtk_box_pack_start(GTK_BOX(vboxMain), frameSamFile, false, false, 0);

    hboxSamFile = gtk_hbox_new(false, 0);
    gtk_container_set_border_width(GTK_CONTAINER(hboxSamFile), 5);
    gtk_container_add(GTK_CONTAINER(frameSamFile), hboxSamFile);

    entrySamFile = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(entrySamFile), 30);
    gtk_entry_set_editable(GTK_ENTRY(entrySamFile), false);
    gtk_box_pack_start(GTK_BOX(hboxSamFile), entrySamFile, true, true, 5);

    buttonBrowseSamFile = gtk_button_new_with_mnemonic("_Browse");
    g_signal_connect (G_OBJECT(buttonBrowseSamFile), "clicked",
        G_CALLBACK(sam_file_browse_event), (gpointer)this);
    gtk_box_pack_start(GTK_BOX(hboxSamFile), buttonBrowseSamFile, false, false, 5);


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
            G_CALLBACK(clearPasswords), (gpointer)this);
    gtk_box_pack_end(GTK_BOX(hboxCommands), buttonClearPasswords, false, false, 0);

    buttonEnableAccounts = gtk_button_new_with_mnemonic("_Enable Accounts");
    g_signal_connect (G_OBJECT (buttonEnableAccounts), "clicked",
            G_CALLBACK(enableAccounts), (gpointer)this);
    gtk_box_pack_end(GTK_BOX(hboxCommands), buttonEnableAccounts, false, false, 0);

    buttonDisableAccounts = gtk_button_new_with_mnemonic("_Disable Accounts");
    g_signal_connect (G_OBJECT (buttonDisableAccounts), "clicked",
            G_CALLBACK(disableAccounts), (gpointer)this);
    gtk_box_pack_end(GTK_BOX(hboxCommands), buttonDisableAccounts, false, false, 0);

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

void owperGUI::sam_file_browse_event(GtkWidget *widget, gpointer owperGUIInstance)
{
    owperGUI *thisOwperGUI = (owperGUI*)owperGUIInstance;

    GtkWidget *fileChooser = gtk_file_chooser_dialog_new ("Open File",
                                GTK_WINDOW(thisOwperGUI->winMain),
                                GTK_FILE_CHOOSER_ACTION_OPEN,
                                GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(fileChooser)) == GTK_RESPONSE_ACCEPT)
    {
        string filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (fileChooser));
        thisOwperGUI->changeHiveFile(filename);
    }

    gtk_widget_destroy (fileChooser);
}

bool owperGUI::changeHiveFile(string newFileName, samHive* newSam/*=NULL*/) {
    gtk_entry_set_text(GTK_ENTRY(entrySamFile), "");
    clearUsers();

    if(sam) {
        delete sam;
    }

    // see if we were passed an already-loaded SAM hive
    // this may be the case if the program was asked to check
    // a hive on startup
    if(newSam != NULL) {
        sam = newSam;
    } else {
        // no, the sam file isn't loaded yet - try to load it
        try {
            sam = new samHive(newFileName.c_str());
        }catch(owpException *exception) {
            //if sam got assigned something, delete it!
            if(sam) {
                delete sam;
                sam = NULL;
            }

            GtkWidget *errorDialog = gtk_message_dialog_new (GTK_WINDOW(this->winMain),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "Error loading hive:\n%s",
                                             exception->errorMessage.c_str());
            gtk_dialog_run (GTK_DIALOG (errorDialog));
            gtk_widget_destroy (errorDialog);
            delete exception;
            return false;
        }
    }

    gtk_entry_set_text(GTK_ENTRY(entrySamFile), newFileName.c_str());
    loadUsers();
    return true;
}

void owperGUI::loadUsers() {
    vector<samUser*> samUserList = sam->getUserList();

    for(unsigned int i = 0; i < samUserList.size(); i++) {
        userWidget *curUserWidget = new userWidget(samUserList.at(i), i);

        vectUserWidgets.push_back(curUserWidget);
        gtk_box_pack_start(GTK_BOX(this->vboxUsers), curUserWidget->getWidget(), false, false, 0);
        gtk_widget_show(curUserWidget->getWidget());
    }
}

void owperGUI::clearUsers() {
    //we are using a vector of pointers, thus the destructors
    //do NOT get called by vector.clear
    for(unsigned int i = 0; i < vectUserWidgets.size(); i++) {
        if(vectUserWidgets.at(i)) {
            delete vectUserWidgets.at(i);
            vectUserWidgets.at(i) = NULL;
        }
    }

    vectUserWidgets.clear();
}

void owperGUI::clearPasswords(GtkWidget *widget, gpointer owperGUIInstance) {
    owperGUI *thisOwperGUI = (owperGUI*)owperGUIInstance;

    for(unsigned int i = 0; i < thisOwperGUI->vectUserWidgets.size(); i++) {
        userWidget *curUserWidget = thisOwperGUI->vectUserWidgets.at(i);
        cout << curUserWidget->getUserName() << endl << flush;
        if(curUserWidget->userIsSelected()) {
            curUserWidget->blankPassword();
            curUserWidget->deselectUser();
            curUserWidget->resetLabel();
        }
    }

    applyChanges("Cleared passwords!", thisOwperGUI);
}

void owperGUI::enableAccounts(GtkWidget *widget, gpointer owperGUIInstance) {
    owperGUI *thisOwperGUI = (owperGUI*)owperGUIInstance;

    for(unsigned int i = 0; i < thisOwperGUI->vectUserWidgets.size(); i++) {
        userWidget *curUserWidget = thisOwperGUI->vectUserWidgets.at(i);
        cout << curUserWidget->getUserName() << endl << flush;
        if(curUserWidget->userIsSelected()) {
            curUserWidget->enableAccount();
            curUserWidget->deselectUser();
            curUserWidget->resetLabel();
        }
    }

    applyChanges("Accounts enabled!", thisOwperGUI);
}

void owperGUI::disableAccounts(GtkWidget *widget, gpointer owperGUIInstance) {
    owperGUI *thisOwperGUI = (owperGUI*)owperGUIInstance;

    for(unsigned int i = 0; i < thisOwperGUI->vectUserWidgets.size(); i++) {
        userWidget *curUserWidget = thisOwperGUI->vectUserWidgets.at(i);
        cout << curUserWidget->getUserName() << endl << flush;
        if(curUserWidget->userIsSelected()) {
            curUserWidget->disableAccount();
            curUserWidget->deselectUser();
            curUserWidget->resetLabel();
        }
    }

    applyChanges("Accounts disabled!", thisOwperGUI);
}

void owperGUI::applyChanges(string successMessage, owperGUI *thisOwperGUI) {
    if(!thisOwperGUI->sam->mergeChangesToHive()) {
        reportMergeFailure(thisOwperGUI);
        return;
    }

    if(!thisOwperGUI->sam->writeHiveToFile()) {
        reportSaveFailure(thisOwperGUI);
        return;
    }

    reportSuccess(successMessage, thisOwperGUI);
}

void owperGUI::reportMergeFailure(owperGUI *thisOwperGUI) {
    string errorMessage  = "Error! Not all data was successfully ";
    errorMessage        += "merged into the hive in memory.  We don't guarantee any particular ";
    errorMessage        += "results at this point.";
    GtkWidget *errorDialog = gtk_message_dialog_new (GTK_WINDOW(thisOwperGUI->winMain),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "%s",
                                             errorMessage.c_str());
    gtk_dialog_run (GTK_DIALOG (errorDialog));
    gtk_widget_destroy (errorDialog);
    return;
}

void owperGUI::reportSaveFailure(owperGUI *thisOwperGUI){
    string errorMessage  = "Error enabling accounts!  Could not write data to hive file on disk. ";
    errorMessage        += "We don't guarantee any particular results at this point.";
    GtkWidget *errorDialog = gtk_message_dialog_new (GTK_WINDOW(thisOwperGUI->winMain),
                                             GTK_DIALOG_DESTROY_WITH_PARENT,
                                             GTK_MESSAGE_ERROR,
                                             GTK_BUTTONS_CLOSE,
                                             "%s",
                                             errorMessage.c_str());
    gtk_dialog_run (GTK_DIALOG (errorDialog));
    gtk_widget_destroy (errorDialog);
    return;
}

void owperGUI::reportSuccess(string taskDesc, owperGUI *thisOwperGUI) {
    GtkWidget *infoDialog = gtk_message_dialog_new (GTK_WINDOW(thisOwperGUI->winMain),
                                                     GTK_DIALOG_DESTROY_WITH_PARENT,
                                                     GTK_MESSAGE_INFO,
                                                     GTK_BUTTONS_CLOSE,
                                                     "%s",
                                                     taskDesc.c_str());
    gtk_dialog_run (GTK_DIALOG (infoDialog));
    gtk_widget_destroy (infoDialog);
}



