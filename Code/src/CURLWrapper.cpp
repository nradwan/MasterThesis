#include <memory>
#include <algorithm>
#include "../include/CURLWrapper.h"
//#include "curl/curl.h"

CURLWrapper::Easy::Easy()
    : mCurl(curl_easy_init()),
    mCode(CURLE_OK)
{
}

CURLWrapper::Easy::~Easy()
{
    curl_easy_cleanup(mCurl);
}

void CURLWrapper::Easy::Init()
{
}

void CURLWrapper::Easy::Cleanup()
{
    curl_easy_cleanup(mCurl);
}

void CURLWrapper::Easy::Reset()
{
    curl_easy_reset(mCurl);
    std::fill(mErrorBuffer, mErrorBuffer + CURL_ERROR_SIZE + 1, 0);
    ErrorBuffer(mErrorBuffer);
    mCode = CURLE_OK;
}

void CURLWrapper::Easy::ErrorBuffer(char* buffer)
{
    SetOption(CURLOPT_ERRORBUFFER, (void*)buffer);
}

void CURLWrapper::Easy::Perform()
{
    mCode = curl_easy_perform(mCurl);
}

bool CURLWrapper::Easy::IsOK()
{
    return mCode == CURLE_OK;
}

CURLcode CURLWrapper::Easy::GetError()
{
    return mCode;
}

std::string CURLWrapper::Easy::GetErrorMessage()
{
    return std::string(mErrorBuffer);
}
