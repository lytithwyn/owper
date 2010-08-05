#include "include/owpException.h"

namespace owper {
    owpException::owpException(string message/* = (char*)""*/, int code/* = 0*/) {
        this->errorMessage = message;
        this->errorCode = code;

        string errorCodeString = stringManip::intToString(errorCode);

        this->formattedMessage = this->errorMessage;
        this->formattedMessage += " [Error#: ";
        this->formattedMessage += errorCodeString;
        this->formattedMessage += "]";

        //int messageLength = errorMessage.size() + errorCodeString.size();

        //this->formattedMessage = new char[messageLength];
        //snprintf(this->formattedMessage, messageLength + 4, "%s [%s]", errorMessage.c_str(), errorCodeString.c_str());
    }

    owpException::~owpException() throw() {
        //if(this->formattedMessage)
        //    delete[] (this->formattedMessage);
    }

    const char* owpException::what() const throw() {
        return this->formattedMessage.c_str();
    }
}
