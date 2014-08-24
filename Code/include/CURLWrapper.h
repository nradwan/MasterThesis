#ifndef CURL_WRAPPER_H
#define CURL_WRAPPER_H

#include <string>
#include "curl/curl.h"

namespace CURLWrapper 
{
    class Easy
    {
    public:
        Easy();
        ~Easy();

        void Init();
        void Cleanup();
        void Reset();

        template <typename T, typename V>
        void SetOption(T option, V parameter)
        {
            curl_easy_setopt(mCurl, option, parameter);
        }

        void Perform();
        bool IsOK();
        CURLcode GetError();
        std::string GetErrorMessage();
    
    private:
        void ErrorBuffer(char* buffer); 
    
    private:
        CURL* mCurl;
        char mErrorBuffer[CURL_ERROR_SIZE + 1];
        CURLcode mCode;
    };

}

#endif //#ifndef CURL_WRAPPER_H
