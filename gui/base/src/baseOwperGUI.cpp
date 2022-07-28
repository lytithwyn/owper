#include "include/baseOwperGUI.h"

const char* baseOwperGUI::WIN_TITLE = "OWPEr - Manage Windows User Accounts";
const char* baseOwperGUI::ERR_MSG_MERGE_FAIL = "Error! Not all data was successfully merged into the hive in memory.  We don't guarantee any particular results at this point.";
const char* baseOwperGUI::ERR_MSG_SAVE_FAIL = "Error! Could not write data to hive file on disk. We don't guarantee any particular results at this point.";
const char* baseOwperGUI::WARN_DFLT_HIVE_MISSING = "DEFAULT hive could not be opened: Clearing Microsoft Account passwords will be disabled";

baseOwperGUI::baseOwperGUI() {
    sam = NULL;
    system = NULL;
    deflt = NULL;
}

// throws an exception if we fail to load the SAM, but continues
// silently if we fail to load one of the optional hives
HIVE_LOAD_RESULT baseOwperGUI::changeHivePath(string newPath) {
    HIVE_LOAD_RESULT loadResult = 0;
    this->clearUsers();

    if(this->sam) {
        delete this->sam;
        this->sam = NULL;
    }

    if(this->system) {
        delete this->system;
        this->system = NULL;
    }

    if(this->deflt) {
        delete this->deflt;
        this->deflt = NULL;
    }

    string samFilePath = stringPrintf( "%s/%s",
        newPath.c_str(),
        findFileCaseInsensitive(newPath, "sam").c_str()
    );

    string systemFilePath = stringPrintf( "%s/%s",
        newPath.c_str(),
        findFileCaseInsensitive(newPath, "system").c_str()
    );

    string defaultFilePath = stringPrintf( "%s/%s",
        newPath.c_str(),
        findFileCaseInsensitive(newPath, "default").c_str()
    );

    try {
        this->sam = new samHive(samFilePath.c_str(), NULL);
    } catch(owpException *e) {
        // make 100% sure SAM is not set and rethrow
        this->sam = NULL;
        throw(e);
    }

    loadResult = HIVE_LOAD_SUCCESS;
    try {
        this->system = new systemHive(systemFilePath.c_str());
        // the bootkey encryption thingy is currently disabled
        // for one thing, this is in the wrong order - we'd have to load
        // SYSTEM before SAM to make this work
        // for another thing, MS changed how this works in Windows 10 at some point
        // so it's just not worth bothering with
        //
        // bootKey = this->system->getBootKey();
        loadResult |= HIVE_LOAD_HAS_SYSTEM;
    } catch(owpException *e) {
        // we don't have a system file...that's OK
        this->system = NULL;
        delete e;
    }

    try {
        this->deflt = new defaultHive(defaultFilePath.c_str());
        loadResult |= HIVE_LOAD_HAS_DEFAULT;
    } catch(owpException *exception) {
        // we don't have a default file...we can't clear MS accounts, but otherwise is OK
        this->deflt = NULL;
        delete exception;
    }

    if(loadResult & HIVE_LOAD_SUCCESS) {
        loadUsers();
    }

    return loadResult;
}

void baseOwperGUI::modifyUsers(const std::function<void(baseUserWidget*, unsigned int)> &modifierFunc, std::string successMessage) {
    for(unsigned int i = 0; i < this->vectUserWidgets.size(); i++) {
        baseUserWidget *curUserWidget = this->vectUserWidgets.at(i);
        if(curUserWidget->userIsSelected()) {
            modifierFunc(curUserWidget, i);
            curUserWidget->deselectUser();
            curUserWidget->resetLabel();
        }
    }

    this->applyChanges(successMessage);
}

void baseOwperGUI::clearPasswords() {
    auto func = [this](baseUserWidget* curUserWidget, unsigned int i) {
        string msAccount = this->sam->getUserAtIndex(i)->getMSAccount();
        if(!msAccount.empty()) {
            this->deflt->deleteStoredIdentity(msAccount);
        }
        this->sam->clearPassword(i);
    };

    this->modifyUsers(func, "Cleared Passwords!");
}

void baseOwperGUI::enableAccounts() {
    auto func = [](baseUserWidget* curUserWidget, unsigned int i) {
        curUserWidget->enableAccount();
    };

    this->modifyUsers(func, "Accounts enabled!");
}

void baseOwperGUI::disableAccounts() {
    auto func = [](baseUserWidget* curUserWidget, unsigned int i) {
        curUserWidget->disableAccount();
    };

    this->modifyUsers(func, "Accounts disabled!");
}

void baseOwperGUI::applyChanges(string successMessage) {
    if(!this->sam->mergeChangesToHive()) {
        this->reportMergeFailure();
        return;
    }

    if(!this->sam->writeHiveToFile(true)) {
        this->reportSaveFailure();
        return;
    }

    if(this->deflt) {
        if(!this->deflt->writeHiveToFile(true)) {
            reportSaveFailure();
            return;
        }
    }

    this->reportSuccess(successMessage);
}

void baseOwperGUI::clearUsers(bool shutdown/*=false*/) {
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

baseOwperGUI::~baseOwperGUI() {
    if(this->sam) {
        delete sam;
    }

    if(this->system) {
        delete system;
    }

    if(this->deflt) {
        delete deflt;
    }
}

