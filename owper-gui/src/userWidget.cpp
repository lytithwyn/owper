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
#include "include/userWidget.h"

userWidget::userWidget(samUser *inSamUser, unsigned int inUserIndex)
{
    user = inSamUser;
    userIndex = inUserIndex;
    chkbtnUser = gtk_check_button_new_with_label("");
    resetLabel();
}

userWidget::~userWidget() {
    gtk_widget_destroy(this->chkbtnUser);
}

void userWidget::resetLabel()
{
    string fullName = "";
    if(user->getFullName() != "") {
        fullName = stringPrintf(" (%s)", user->getFullName().c_str());
    }

    string isBlank = "";
    if(user->passwordIsBlank()) {
        isBlank = " [Blank]";
    }

    string isDisabled = "";
    if(user->accountIsDisabled()) {
        isDisabled = " [Disabled]";
    }

    string label = stringPrintf("%s%s%s%s", user->getUserName().c_str(), fullName.c_str(), isBlank.c_str(), isDisabled.c_str());
    gtk_button_set_label(GTK_BUTTON(this->chkbtnUser), label.c_str());
}
