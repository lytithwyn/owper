#include "include/stringManip.h"


namespace stringManip {
    /**
     * Take a string, and return a copy of it all lower case
     * @param string The string to convert to lower case
     * @return string The string in all lower case
     */
    string stringToLower(string inString) {
        string stringLower = inString; // to fix freakiness in transform
        transform(inString.begin(), inString.end(), stringLower.begin(),
            static_cast < int(*)(int) > (tolower));

        return stringLower;
    }

    /**
     * Take an int and convert it to a string
     * @param int to convert
     * @return string
     */
    string intToString(int num)
    {
        char *castingString;
        int len;

        len = snprintf(NULL, 0, "%d", num);

        if(!(castingString = (char *)calloc(1, (len + 1) * sizeof(char))))
                return string((char *)"");

        snprintf(castingString, len + 1, "%d", num);

        return string(castingString);
    }
}
