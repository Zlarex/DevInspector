/**
 * @author Zlarex - 22/10/2021
 * 
 * Managest
 * Simple program to determine stuff related about the system information.
 * Currently only intended for windows operating system only.
 * 
 * https://github.com/Zlarex/Managest
 */

#include <iostream>
#include <string>
#include <windows.h>
#include <sddl.h>
#include "all.h"

using namespace std;

int main(int argc, char const *argv[])
{
    // const char* sid = getSID();
    // cout << "SID: " << sid << endl;
    // const char* rid = getRID();
    // cout << "RID: " << rid << endl;
    const char* mac = getMAC();
    long long int ramSize = getRAMSize();
    cout << "MAC: " << mac << endl;
    cout << "RAM Size: " << ramSize / 1048567 << endl;
    return 0;
}
