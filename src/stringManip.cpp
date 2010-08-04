#include "include/stringManip.h"


namespace stringManip{
    /**
     * Take a string, and return a copy of it all lower case
     * @param string The string to convert to lower case
     * @return string The string in all lower case
     */
    string stringToLower(string inString){
        string stringLower = inString; // to fix freakiness in transform
        transform(inString.begin(), inString.end(), stringLower.begin(),
            static_cast < int(*)(int) > (tolower));

        return stringLower;
    }
}
