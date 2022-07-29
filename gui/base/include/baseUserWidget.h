#ifndef BASE_USER_WIDGET_H
#define BASE_USER_WIDGET_H

#include <string>

#include "include/samUser.h"
#include "include/stringManip.h"

using namespace std;
using owper::samUser;
using stringManip::stringPrintf;

class baseUserWidget
{
    protected:
        samUser *user;
        unsigned int userIndex;

    public:
        baseUserWidget(samUser *inSamUser, unsigned int inUserIndex) { this->user = inSamUser; this->userIndex = inUserIndex; };
        virtual ~baseUserWidget(){ };

        virtual void resetLabel()      { };
        unsigned int getUserIndex()    { return userIndex; };
        string       getUserName()     { return user->getUserName(); };
        string       getFullName()     { return user->getFullName(); };
        bool         passwordIsBlank() { return this->user->passwordIsBlank(); };
        virtual bool userIsSelected()  { return false; };
        void         enableAccount()   { user->enableAccount(); };
        void         disableAccount()  { user->disableAccount(); };
        virtual void selectUser()      { };
        virtual void deselectUser()    { };
        virtual void destroyWidget()   { };

};



#endif

