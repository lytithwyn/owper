#include "include/fileManip.h"

namespace fileManip{
    /**
     * Find a file in the specified directory, ignoring case.
     * @param string directory The directory in which to search
     * @param string fileName The file name to look for (case ignored)
     * @return string The file name with the proper case (as it exists in the filesystem) or an empty string if it wasn't found.
     */
    string findFileCaseInsensitive(string directory, string fileName){
        DIR *dirTest;
        dirent *dirEntry;

        //make sure "directory" exists and is a directory
        if(!(dirTest = opendir(directory.c_str()))){
            return ""; //not a directory
        }

        //convert the filename we were given to lower case before searching
        string fileNameLower = stringToLower(fileName);

        string testFileName;
        string testFileNameLower;
        while((dirEntry = readdir(dirTest)))
        {
            testFileName.erase();
            testFileNameLower.erase();

            testFileName = dirEntry->d_name;
            testFileNameLower = stringToLower(testFileName);

            if(testFileNameLower == fileNameLower)
                return testFileName;
        }

        return "";

    }
}
