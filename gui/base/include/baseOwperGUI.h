#ifndef BASE_GUI_H
#define BASE_GUI_H

#include <functional>
#include <string>

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

        void loadGUI(){ };
        void modifyUsers(const std::function<void(baseUserWidget*, unsigned int)> &modifierFunc, std::string successMessage);

        static const char* WIN_TITLE;
        static const char* ERR_MSG_SAVE_FAIL;
        static const char* ERR_MSG_MERGE_FAIL;
        static const char* WARN_DFLT_HIVE_MISSING;
    public:
        static const HIVE_LOAD_RESULT HIVE_LOAD_RESULT_UNDEF = 0;
        static const HIVE_LOAD_RESULT HIVE_LOAD_SUCCESS = 1;
        static const HIVE_LOAD_RESULT HIVE_LOAD_HAS_SYSTEM = 1<<1;
        static const HIVE_LOAD_RESULT HIVE_LOAD_HAS_DEFAULT = 1<<2;

        baseOwperGUI();
        ~baseOwperGUI();

        virtual HIVE_LOAD_RESULT changeHivePath(string newPath);
        virtual void clearUsers(bool isShutdown = false);
        virtual void loadUsers(){ };
        void clearPasswords();
        void enableAccounts();
        void disableAccounts();
        void applyChanges(string successMessage);
        virtual void reportSuccess(string successMessage){ };
        virtual void reportMergeFailure(){ };
        virtual void reportSaveFailure(){ };
};

#endif
