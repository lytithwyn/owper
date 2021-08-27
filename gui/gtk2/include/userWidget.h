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
#ifndef USER_WIDGET_H
#define USER_WIDGET_H

#include <gtk/gtk.h>
#include <string>

#include "include/samUser.h"
#include "include/stringManip.h"
#include "include/baseUserWidget.h" // from ../gui/base

using namespace std;
using owper::samUser;
using stringManip::stringPrintf;

class userWidget : public baseUserWidget
{
    private:
        GtkWidget* chkbtnUser;

    public:
        userWidget(samUser *inSamUser, unsigned int inUserIndex);

        void         resetLabel();
        bool         userIsSelected()  { return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(chkbtnUser)); };
        GtkWidget   *getWidget()       { return this->chkbtnUser; };
        void         destroyWidget()   { gtk_widget_destroy(this->chkbtnUser); };
        void         selectUser()      { gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkbtnUser), true); };
        void         deselectUser()    { gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(chkbtnUser), false); };

};



#endif
