#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h> 
#include <vector>


DWORD dwGetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID)
{
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry32))
    {
        do
        {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0)
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32));

    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}


int main()
{
    reload:
    system("CLS");
    HWND hGameWindow = FindWindow(NULL, "Among Us");
    if (hGameWindow != NULL)
    {
        std::cout << "Window found successfully!" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
    }
    else
    {
        std::cout << "Unable to find the window!" << std::endl;
    }
    DWORD pID;
    GetWindowThreadProcessId(hGameWindow, &pID);
    HANDLE processHandle = NULL;
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (processHandle == INVALID_HANDLE_VALUE || processHandle == NULL)
    {
        std::cout << "Failed to open process!" << std::endl;
        system("pause");
        return 0;
    }

    char moduleName[] = "GameAssembly.dll";
    DWORD gameBaseAddress = dwGetModuleBaseAddress(_T(moduleName), pID);
    DWORD offsetGameToBaseAddress = 0x01C57F7C;
    std::vector<DWORD> pointsOffsets{0x5C, 0x0, 0x5C, 0x2C, 0x8, 0x5C, 0x2C};
    DWORD baseAddress;

    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress + offsetGameToBaseAddress), &baseAddress, sizeof(baseAddress), NULL);
    std::cout << "Debugginfo: Baseaddress = " << std::hex << baseAddress << std::endl;
    DWORD pointsAddress = baseAddress;
    for (int i = 0; i < pointsOffsets.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(pointsAddress + pointsOffsets.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
        std::cout << "Debugginfo: address at offset = " << std::hex << pointsAddress << std::endl;
    }
    pointsAddress += pointsOffsets.at(pointsOffsets.size() - 1);
    std::cout << "Debugginfo: address at final offset = " << std::hex << pointsAddress << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;


    //second offsets 
    DWORD pointsAddress2 = baseAddress;
    std::vector<DWORD> pointsOffsets2{0x5C, 0x0, 0x5C, 0x2C, 0x8, 0x5C, 0x30};
    for (int i = 0; i < pointsOffsets2.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(pointsAddress2 + pointsOffsets2.at(i)), &pointsAddress2, sizeof(pointsAddress2), NULL);
        std::cout << "Debugginfo: address at offset = " << std::hex << pointsAddress2 << std::endl;
    }
    pointsAddress2 += pointsOffsets2.at(pointsOffsets2.size() - 1);
    std::cout << "Debugginfo: address at final offset2 = " << std::hex << pointsAddress2 << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;

    //"UI"
    std::cout << "Y- UP " << std::endl;
    std::cout << "H- DOWN" << std::endl;
    std::cout << "G- LEFT" << std::endl;
    std::cout << "J- RIGHT" << std::endl;
    std::cout << "T- Reloads the cheat" << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    float xValue = 0;
    float yValue = 0;
    
    while (true)
    {
        Sleep(30);

        if (GetAsyncKeyState(0x47)) // G left
        {
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress), &xValue, sizeof (float), NULL);
            std::cout << "Current X value: " << std::hex << xValue << std::endl;
            xValue -=0.5;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &xValue, sizeof(float), 0);
        }
        if (GetAsyncKeyState(0x4A)) // J right 
        { 
           
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress), &xValue, sizeof (float), NULL);
            std::cout << "Current X value: " << std::hex << xValue << std::endl;
            xValue +=0.5;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &xValue, sizeof(float), 0);
        }
        if (GetAsyncKeyState(0x59)) // Y up
        {
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress2), &yValue, sizeof (float), NULL);
            std::cout << "Current Y value: " << std::hex << yValue << std::endl;
            yValue +=0.5;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress2), &yValue, sizeof(float), 0);
        }
        if (GetAsyncKeyState(0x48)) // H down
        {
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress2), &yValue, sizeof (float), NULL);
            std::cout << "Current Y value: " << std::hex << yValue << std::endl;
            yValue -=0.5;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress2), &yValue, sizeof(float), 0);
        }
        if (GetAsyncKeyState(0x54)) // T reload
        {
            goto reload;
        }

    }

}
