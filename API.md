# UAC Bypass API Documentation

## Overview

This document describes the API and classes used in the UAC Bypass tool.

## Classes

### RegistryKey

A RAII wrapper for Windows registry operations.

#### Public Methods

- `RegistryKey()`: Default constructor
- `~RegistryKey()`: Destructor that automatically closes the registry key
- `void Open(HKEY rootKey, const wchar_t* subKey, REGSAM access)`: Opens an existing registry key
- `void Create(HKEY rootKey, const wchar_t* subKey, REGSAM access)`: Creates a new registry key
- `void SetValue(const wchar_t* valueName, const wchar_t* data)`: Sets a registry value
- `HKEY GetHandle() const`: Returns the underlying registry key handle

### UACBypass

Main class implementing the UAC bypass functionality.

#### Public Methods

- `static void Execute(const std::wstring& command)`: Executes a command with elevated privileges

#### Static Constants

- `SETTINGS_KEY`: Registry path for ms-settings protocol
- `FODHELPER_PATH`: Path to fodhelper.exe

## Error Handling

All methods throw `std::runtime_error` with descriptive messages in case of failure.

## Example Usage

```cpp
try {
    UACBypass::Execute(L"C:\\Windows\\System32\\cmd.exe");
} catch (const std::exception& e) {
    // Handle error
}
```

## Dependencies

- Windows API (windows.h)
- Standard C++ Library
  - memory
  - string
  - stdexcept
  - chrono
  - thread

## Security Notes

- All registry operations are performed under HKEY_CURRENT_USER
- Registry keys are automatically cleaned up through RAII
- Exception handling ensures proper resource cleanup 