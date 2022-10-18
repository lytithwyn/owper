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

#include <string>

// ncurses stuff
#include <menu.h>

#include "include/samUser.h"
#include "include/stringManip.h"
#include "include/baseUserWidget.h" // from ../../gui/base

using namespace std;
using owper::samUser;
using stringManip::stringPrintf;

class userWidget : public baseUserWidget
{
    private:
        std::string label;
        ITEM* menuItem;

    public:
        userWidget(samUser *inSamUser, unsigned int inUserIndex);
        ~userWidget();

        void         resetLabel();
        ITEM*        getMenuItem() { return this->menuItem; };
        bool         userIsSelected()  { return item_value(this->menuItem); };
        void         destroyWidget()   {  };
        void         selectUser()      {  };
        void         deselectUser()    {  };

};



#endif
