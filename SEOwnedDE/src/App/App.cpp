#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "App.h"

#include "Hooks/WINAPI_WndProc.h"

#include "Features/Materials/Materials.h"
#include "Features/Outlines/Outlines.h"
#include "Features/WorldModulation/WorldModulation.h"
#include "Features/Paint/Paint.h"
#include "Features/Menu/Menu.h"
#include "Features/Players/Players.h"


// this guid check could be better but i don't really mind cos it works
std::string GetMachineGuid() {
    HKEY hKey;
    const char* subkey = "SOFTWARE\\Microsoft\\Cryptography";
    const char* valueName = "MachineGuid";
    char machineGuid[37];
    DWORD bufferSize = sizeof(machineGuid);

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, subkey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, valueName, nullptr, nullptr, (LPBYTE)machineGuid, &bufferSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return std::string(machineGuid);
        }
        RegCloseKey(hKey);
    }
    return "";
}

const std::vector<std::string> validMachineGuids = {
    "1479d7a4-5f9b-4c4c-bec3-673a0589bd5d", // grizz
    "cd259989-41f4-4cbc-929a-7caf0e7fa67c", // deadly
    "0a8cf01a-db0d-4c92-a4b3-0ea3d7ba2f9c" // narrow
};

bool IsValidMachineGuid(const std::string& machineGuid) {
    return std::find(validMachineGuids.begin(), validMachineGuids.end(), machineGuid) != validMachineGuids.end();
}

void CApp::Start()
{
    std::string machineGuid = GetMachineGuid();

	// well it crashes if !IsValidMachineGuid but i decided to add this cos why not
    if (!IsValidMachineGuid(machineGuid)) {
        I::CVar->ConsoleColorPrintf({ 255, 0, 0, 255 }, "Unauthorized machine! Exiting...\n");
        return;
    }

    while (!Memory::FindSignature("client.dll", "48 8B 0D ? ? ? ? 48 8B 10 48 8B 19 48 8B C8 FF 92"))
    {
        bUnload = GetAsyncKeyState(VK_F11) & 0x8000;
        if (bUnload)
            return;

        Sleep(500);
    }

    U::Storage->Init("midnight");
    U::SignatureManager->InitializeAllSignatures();
    U::InterfaceManager->InitializeAllInterfaces();

    H::Draw->UpdateScreenSize();

    H::Fonts->Reload();

    if (I::EngineClient->IsInGame() && I::EngineClient->IsConnected())
    {
        H::Entities->UpdateModelIndexes();
    }

    U::HookManager->InitializeAllHooks();

    Hooks::WINAPI_WndProc::Init();

    F::Players->Parse();

    Config::Load(U::Storage->GetConfigFolder() / "default.json");

    const auto month = []
        {
            const std::time_t t = std::time(nullptr);
            tm Time = {};
            localtime_s(&Time, &t);

            return Time.tm_mon + 1;
        }();

    Color_t msgColor = { 197, 108, 240, 255 };
    if (month == 10)
    {
        I::MatSystemSurface->PlaySound("vo\\halloween_boss\\knight_alert.mp3");
        msgColor = { 247, 136, 18, 255 };
    }
    else if (month == 12 || month == 1 || month == 2)
    {
        if (month == 12)
        {
            I::MatSystemSurface->PlaySound("misc\\jingle_bells\\jingle_bells_nm_04.wav");
        }

        msgColor = { 28, 179, 210, 255 };
    }
    I::EngineClient->ClientCmd_Unrestricted("clear");
    Sleep(25);
    I::CVar->ConsoleColorPrintf(msgColor, "midnight loaded!\n");
}

void CApp::Loop()
{
    while (true)
    {
        bool bShouldUnload = GetAsyncKeyState(VK_F11) & 0x8000 && SDKUtils::IsGameWindowInFocus() || bUnload;
        if (bShouldUnload)
            break;

        Sleep(50);
    }
}

void CApp::Shutdown()
{
    if (!bUnload)
    {
        U::HookManager->FreeAllHooks();

        Hooks::WINAPI_WndProc::Release();

        Sleep(250);

        F::Materials->CleanUp();
        F::Outlines->CleanUp();
        F::Paint->CleanUp();

        F::WorldModulation->RestoreWorldModulation();

        if (const auto cl_wpn_sway_interp{ I::CVar->FindVar("cl_wpn_sway_interp") })
        {
            cl_wpn_sway_interp->SetValue(0.0f);
        }

        if (F::Menu->IsOpen())
        {
            I::MatSystemSurface->SetCursorAlwaysVisible(false);
        }
    }
    I::EngineClient->ClientCmd_Unrestricted("clear");
    Sleep(25);
    I::CVar->ConsoleColorPrintf({ 255, 70, 70, 255 }, "midnight unloaded!\n");
}
