#include "inspect.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <sddl.h>
#include <winbase.h>
#include <iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")

const char* getSID()
{
    HANDLE hToken = NULL;
    PTOKEN_USER pUser = NULL;
    DWORD bufferLen = 0;
    OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
    GetTokenInformation(hToken, TokenUser, NULL, 0, &bufferLen);
    
    pUser = (PTOKEN_USER) malloc(bufferLen);
    memset(pUser, 0, bufferLen);
    GetTokenInformation(hToken, TokenUser, pUser, bufferLen, &bufferLen);
    
    PSID psid = pUser->User.Sid;
    LPSTR strsid = NULL;
    ConvertSidToStringSidA(psid, &strsid);
    
    if (!IsValidSid(psid)) return NULL;
    
    int len = strlen(strsid);
    const char* retVal = (const char*) malloc(len + 1);
    memset((void*)retVal, 0, len + 1);
    memcpy((void*)retVal, strsid, len);
    CloseHandle(hToken);
    free(pUser);
    return retVal;
}

const char* getRID()
{
    const char* sid = getSID();
    int lastElement = strlen(sid);
    int found = -1;
    
    for (int i = lastElement; i > 0; i--)
    {
        if (sid[i - 1] == '-')
        {
            found = i - 1;
            break;
        }
    }
    if (found == -1) return NULL;

    const char* retVal = (const char*) malloc(lastElement - found);
    memset((void*)retVal, 0, lastElement - found);
    memcpy((void*)retVal, sid + found + 1, lastElement - found);
    free((void*)sid);

    return retVal;
}

const char* getMAC()
{
    IP_ADAPTER_INFO adapterInfo[16] = {0};
    DWORD bufferLen = sizeof(adapterInfo);
    const DWORD status = GetAdaptersInfo(adapterInfo, &bufferLen);
    PIP_ADAPTER_INFO pai = adapterInfo;

    char* retVal = (char*) malloc(strlen((const char*)pai->Address));
    sprintf(retVal, "%02X:%02X:%02X:%02X:%02X:%02X",
    adapterInfo->Address[0], adapterInfo->Address[1],
    adapterInfo->Address[2], adapterInfo->Address[3],
    adapterInfo->Address[4], adapterInfo->Address[5]);
    return (const char*)retVal;
}

long long int getRAMSize()
{
    MEMORYSTATUSEX memory;
    memset(&memory, 0, sizeof(memory));
    memory.dwLength = sizeof(memory);
    GlobalMemoryStatusEx(&memory);
    return memory.ullTotalPhys;
}