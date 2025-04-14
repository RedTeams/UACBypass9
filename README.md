# UAC Bypass Tool

A Windows UAC (User Account Control) bypass tool that utilizes the fodhelper.exe method to execute commands with elevated privileges.

## Description

This tool demonstrates a UAC bypass technique by manipulating registry keys and utilizing the Windows fodhelper.exe application. It's designed for educational purposes to understand Windows security mechanisms.

## Features

- Modern C++ implementation with RAII principles
- Exception-based error handling
- Clean and maintainable code structure
- Registry manipulation with proper cleanup
- Detailed error reporting

## Requirements

- Windows 10 or later
- Visual Studio 2019 or later (for building)
- Administrator privileges (for execution)

## Building

1. Open the solution in Visual Studio
2. Select Release configuration
3. Build the solution

## Usage

```powershell
UacBypass.exe <file.exe or command>
```

Example:
```powershell
UacBypass.exe "C:\Windows\System32\cmd.exe"
```

## Security Considerations

- This tool should only be used for educational purposes
- Always test in a controlled environment
- Be aware of potential security implications
- Use responsibly and ethically

## Technical Details

The tool works by:
1. Creating/modifying registry keys under HKEY_CURRENT_USER
2. Setting up command execution through ms-settings protocol
3. Triggering fodhelper.exe to execute the command with elevated privileges

## Error Handling

The tool provides detailed error messages for:
- Registry operation failures
- Permission issues
- User cancellation
- Unexpected errors

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Disclaimer

This tool is provided for educational purposes only. The authors are not responsible for any misuse or damage caused by this tool.
