#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <tchar.h> 
#include <vector>
#include <thread>

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
        std::cout << "Among Us found successfully!" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
    }
    else
    {
        std::cout << "Unable to find Among Us, Please open Among Us!" << std::endl;
        std::cout << "------------------------------------------------------" << std::endl;
        Sleep(1000);
        std::cout << "Auto reloading in 5 seconds!" << std::endl;
        Sleep(1000);
        std::cout << "Auto reloading in 4 seconds!" << std::endl;
        Sleep(1000);
        std::cout << "Auto reloading in 3 seconds!" << std::endl;
        Sleep(1000);
        std::cout << "Auto reloading in 2 seconds!" << std::endl;
        Sleep(1000);
        std::cout << "Auto reloading in 1 seconds!" << std::endl;
        
        goto reload;
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
    DWORD offsetGameToBaseAddress = 0x028EDF98;
    std::vector<DWORD> pointsOffsets{0x5C, 0x0, 0x60, 0x30, 0x8, 0x5C, 0x2C};
    DWORD baseAddress;

    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress + offsetGameToBaseAddress), &baseAddress, sizeof(baseAddress), NULL);
    //std::cout << "Debugginfo: Baseaddress = " << std::hex << baseAddress << std::endl;
    DWORD pointsAddress = baseAddress;
    for (int i = 0; i < pointsOffsets.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(pointsAddress + pointsOffsets.at(i)), &pointsAddress, sizeof(pointsAddress), NULL);
        //std::cout << "Debugginfo: address at offset = " << std::hex << pointsAddress << std::endl;
    }
    pointsAddress += pointsOffsets.at(pointsOffsets.size() - 1);
   //std::cout << "Debugginfo: address at final offset = " << std::hex << pointsAddress << std::endl;
   //std::cout << "------------------------------------------------------" << std::endl;


    //second offsets Y
    DWORD pointsAddress2 = baseAddress;
    std::vector<DWORD> pointsOffsets2{0x5C, 0x0, 0x60, 0x30, 0x8, 0x5C, 0x30};
    for (int i = 0; i < pointsOffsets2.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(pointsAddress2 + pointsOffsets2.at(i)), &pointsAddress2, sizeof(pointsAddress2), NULL);
        //std::cout << "Debugginfo: address at offset = " << std::hex << pointsAddress2 << std::endl;
    }
    pointsAddress2 += pointsOffsets2.at(pointsOffsets2.size() - 1);
    //std::cout << "Debugginfo: address at final offset2 = " << std::hex << pointsAddress2 << std::endl;
    //std::cout << "------------------------------------------------------" << std::endl;

    //"UI"
    std::cout << "W, UP ARROW - UP " << std::endl;
    std::cout << "S, DOWN ARROW - DOWN" << std::endl;
    std::cout << "A, LEFT ARROW - LEFT" << std::endl;
    std::cout << "D, RIGHT ARROW - RIGHT" << std::endl;
    std::cout << "T - Reloads the cheat" << std::endl;
    std::cout << "Y - Set speed, the default speed is 0.5" << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;
    float xValue = 0;
    float yValue = 0;
    float speed= 0.5;

    while (true)
    {
        Sleep(30);

        // a,d,w,s keys
        if (GetAsyncKeyState(0x41)) // A left
        {
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress), &xValue, sizeof (float), NULL);
            std::cout << "Current X value: " << std::hex << xValue << std::endl;
            xValue -=speed;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &xValue, sizeof(float), 0);
        }
        if (GetAsyncKeyState(0x44)) // D right 
        { 
           
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress), &xValue, sizeof (float), NULL);
            std::cout << "Current X value: " << std::hex << xValue << std::endl;
            xValue +=speed;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &xValue, sizeof(float), 0);
        }
        if (GetAsyncKeyState(0x57)) // W up
        {
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress2), &yValue, sizeof (float), NULL);
            std::cout << "Current Y value: " << std::hex << yValue << std::endl;
            yValue +=speed;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress2), &yValue, sizeof(float), 0);
        }
        if (GetAsyncKeyState(0x53)) // H down
        {
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress2), &yValue, sizeof (float), NULL);
            std::cout << "Current Y value: " << std::hex << yValue << std::endl;
            yValue -=speed;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress2), &yValue, sizeof(float), 0);
        }
        // a,d,w,s keys end

        // arrow keys
        if (GetAsyncKeyState(VK_LEFT)) // LEFT ARROW
        {
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress), &xValue, sizeof (float), NULL);
            std::cout << "Current X value: " << std::hex << xValue << std::endl;
            xValue -=speed;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &xValue, sizeof(float), 0);
        }
        if (GetAsyncKeyState(VK_RIGHT)) // RIGHT ARROW
        { 
           
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress), &xValue, sizeof (float), NULL);
            std::cout << "Current X value: " << std::hex << xValue << std::endl;
            xValue +=speed;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress), &xValue, sizeof(float), 0);
        }
        if (GetAsyncKeyState(VK_UP)) // UP ARROW
        {
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress2), &yValue, sizeof (float), NULL);
            std::cout << "Current Y value: " << std::hex << yValue << std::endl;
            yValue +=speed;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress2), &yValue, sizeof(float), 0);
        }
        if (GetAsyncKeyState(VK_DOWN)) // DOWN ARROW
        {
            ReadProcessMemory(processHandle, (LPCVOID)(pointsAddress2), &yValue, sizeof (float), NULL);
            std::cout << "Current Y value: " << std::hex << yValue << std::endl;
            yValue -=speed;
           
            WriteProcessMemory(processHandle, (LPVOID)(pointsAddress2), &yValue, sizeof(float), 0);
        }
        // arrow keys end


        if (GetAsyncKeyState(0x59)) // Y set speed 
        {
            std::cout << "Set speed:" << std::endl;
            std::cin >> speed;
        }

        if (GetAsyncKeyState(0x54)) // T reload
        {
            goto reload;
        }

    }

}
