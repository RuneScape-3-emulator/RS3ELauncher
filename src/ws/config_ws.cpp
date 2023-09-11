//
// Created by david on 2023-09-08.
//

#include "config_ws.h"
#include <curl/curl.h>

static CHAR Prefix[1024] = {0};
static CHAR Suffix[1024] = {0};
static CHAR ParamPrefix[1024] = {0};
static CHAR ParamSuffix[1024] = {0};
static CHAR Fmt[4096] = {0};

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    memcpy(userp, contents, total_size);
    return total_size;
}

bool requestSession(char* RequestArgs)
{
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    if (!curl) {
        return false;
    }

    char* data = new char[10000];

    curl_easy_setopt(curl, CURLOPT_URL, "http://www.runescape.com/l=0/jav_config.ws?binaryType=2");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        delete[] data;
        return false;
    }

    Prefix[0] = 0;
    Suffix[0] = 0;
    ParamPrefix[0] = 0;
    ParamSuffix[0] = 0;
    Fmt[0] = 0;

    char *NextLine = data;
    while (NextLine) {
        char *Middle = strchr(NextLine, '=');
        char *Next = strstr(NextLine + 2, "\n");
        char *RealNext = Next;
        if (!RealNext) {
            RealNext = data + strlen(data);
        }

        if (Middle) {
            size_t PrefixLength = (size_t) Middle - (size_t) NextLine;
            if (PrefixLength >= sizeof(Prefix)) {
                goto _cont;
            }

            memcpy(Prefix, NextLine, PrefixLength);
            Prefix[PrefixLength] = 0;

            size_t SuffixLength = (size_t) RealNext - (size_t) Middle - 1;
            if (SuffixLength >= sizeof(Suffix)) {
                goto _cont;
            }

            memcpy(Suffix, Middle + 1, SuffixLength);
            Suffix[SuffixLength] = 0;

            if (!strcmp(Prefix, "param")) {
                char *ParamMiddle = strchr(Suffix, '=');
                if (ParamMiddle) {
                    size_t ParamPrefixLength = (size_t) ParamMiddle - (size_t) Suffix;
                    memcpy(ParamPrefix, Suffix, ParamPrefixLength);
                    ParamPrefix[ParamPrefixLength] = 0;

                    size_t ParamSuffixLength = SuffixLength - ParamPrefixLength - 1;
                    memcpy(ParamSuffix, ParamMiddle + 1, ParamSuffixLength);
                    ParamSuffix[ParamSuffixLength] = 0;

                    if ((ParamPrefixLength + ParamSuffixLength + 1) >= sizeof(Fmt)) {
                        goto _cont;
                    }

                    sprintf(Fmt, R"("%s" "%s" )", ParamPrefix, ParamSuffix);
                    strcat_s(RequestArgs, 5000, Fmt);
                }
            }
        }

        _cont:
        NextLine = Next;
        if (NextLine) {
            NextLine += 1;
        }
    }

    curl_easy_cleanup(curl);
    delete[] data;
    return true;
}

