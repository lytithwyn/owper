#ifndef BASE_GUI_H
#define BASE_GUI_H

#include "include/samHive.h"
#include "include/systemHive.h"
#include "include/defaultHive.h"
#include "include/stringManip.h"
#include "include/baseUserWidget.h"

typedef unsigned int HIVE_LOAD_RESULT;

using namespace owper;
using namespace fileManip;

class baseOwperGUI {
    protected:
        string stringHivePath;
        samHive     *sam;
        systemHive  *system;
        defaultHive *deflt;
        std::vector<baseUserWidget*> vectUserWidgets;

        static const HIVE_LOAD_RESULT HIVE_LOAD_SUCCESS = 1;
        static const HIVE_LOAD_RESULT HIVE_LOAD_HAS_SYSTEM = 1<<1;
        static const HIVE_LOAD_RESULT HIVE_LOAD_HAS_DEFAULT = 1<<2;

        virtual void loadGUI(){ };
    public:
        baseOwperGUI(string stringInitHivePath = "");
        ~baseOwperGUI();

        virtual HIVE_LOAD_RESULT changeHivePath(string newPath);
        virtual void clearUsers(bool isShutdown = false){ };
        virtual void loadUsers(){ };
        void applyChanges(string successMessage);
        virtual void reportSuccess(string successMessage){ };
        virtual void reportMergeFailure(){ };
        virtual void reportSaveFailure(){ };
};

#endif
