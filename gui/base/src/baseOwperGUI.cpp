#include "include/baseOwperGUI.h"

baseOwperGUI::baseOwperGUI(string initHivePath/*=""*/) {
    sam = NULL;
    system = NULL;
    deflt = NULL;

    if(!initHivePath.empty()) {
        this->changeHivePath(initHivePath);
    }
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

    unsigned char* bootKey = NULL;
    try {
        this->sam = new samHive(samFilePath.c_str(), bootKey);
    } catch(owpException *e) {
        // make 100% sure SAM is not set and rethrow
        this->sam = NULL;
        throw(e);
    }

    loadResult = HIVE_LOAD_SUCCESS;
    try {
        this->system = new systemHive(systemFilePath.c_str());
        bootKey = this->system->getBootKey();
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
