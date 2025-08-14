# FileTrail

FileTrail is a Windows Shell Extension that enhances file management capabilities by adding custom property pages and context menu options to Windows Explorer. Built as a COM-based shell extension using ATL (Active Template Library), it integrates seamlessly with the Windows shell to provide additional file information and operations.

## Features

- **Property Page Extension**: Adds a custom "File Trail" tab to file and folder properties dialog
- **Multi-File Support**: Works with single files, multiple selections, directories, and drives
- **Shell Integration**: Seamlessly integrates with Windows Explorer context menus
- **File Information Display**: Shows detailed information about selected files
- **Unicode Support**: Full Unicode character support for international file names
- **Modern Windows Support**: Compatible with Windows 10 and later versions

## System Requirements

- Windows 10 or later (x64 recommended)
- Visual Studio 2019 or later for building from source
- Administrative privileges for installation/registration

## Installation

### Pre-built Release
1. Download the latest release from the releases page
2. Right-click on the downloaded DLL file and select "Run as administrator"
3. Use `regsvr32` to register the shell extension:
   ```cmd
   regsvr32 FileTrail.dll
   ```

### Building from Source
1. Clone this repository:
   ```bash
   git clone <repository-url>
   cd FileTrail
   ```

2. Open `FileTrail.sln` in Visual Studio

3. Select your target configuration:
   - **Debug/Release**: Choose based on your needs
   - **x64**: Recommended for modern systems
   - **Win32**: For 32-bit compatibility if needed

4. Build the solution (`Ctrl+Shift+B`)

5. Register the built DLL:
   ```cmd
   regsvr32 x64\Release\FileTrail.dll
   ```

## Usage

After installation, FileTrail automatically integrates with Windows Explorer:

### Property Pages
1. Right-click on any file, folder, or drive in Windows Explorer
2. Select "Properties" from the context menu
3. Look for the "File Trail" tab in the Properties dialog
4. Click the tab to view additional file information and options

### Supported File Types
- Individual files (`*`)
- Multiple file selections
- Directories and folders
- Drive roots
- All file system objects

## Architecture

FileTrail is implemented as a COM server with the following key components:

- **CFileTrailShlExt**: Main COM class implementing shell extension interfaces
- **IShellExtInit**: Handles initialization with selected files/folders
- **IShellPropSheetExt**: Manages property page creation and lifecycle
- **IContextMenu**: Provides context menu integration
- **DialogProc**: Handles the property page dialog messages
- **PropSheetPageProc**: Manages property page lifecycle callbacks

### Class ID
```
{1B12EB41-A8BD-42F4-8EF6-A3E1BE205172}
```

## Development

### Project Structure
```
FileTrail/
├── FileTrail.cpp          # Main COM implementation
├── FileTrail.h            # ATL module definitions
├── MainDialog.cpp         # Dialog implementation (legacy)
├── MainDialog.h           # Dialog class definitions
├── dllmain.cpp           # DLL entry points
├── resource.h            # Resource definitions
├── FileTrail.rc          # Resource file
├── FileTrail.idl         # Interface definitions
├── Settings.cpp/h        # Configuration management
├── utl.cpp/h            # Utility functions
└── wnd.h                # Window management helpers
```

### Key Interfaces Implemented
- `IShellExtInit` - Shell extension initialization
- `IShellPropSheetExt` - Property sheet extension
- `IContextMenu` - Context menu extension

### Registry Entries
The shell extension registers itself in several locations:
- `HKEY_CLASSES_ROOT\*\shellex\PropertySheetHandlers\FileTrail`
- `HKEY_CLASSES_ROOT\AllFileSystemObjects\shellex\PropertySheetHandlers\FileTrail`
- `HKEY_CLASSES_ROOT\Directory\shellex\PropertySheetHandlers\FileTrail`
- `HKEY_CLASSES_ROOT\Drive\shellex\PropertySheetHandlers\FileTrail`
- `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Shell Extensions\Approved`

## Troubleshooting

### Common Issues

**Shell extension not appearing:**
1. Ensure the DLL is properly registered with `regsvr32`
2. Check that you have administrative privileges
3. Restart Windows Explorer (`taskkill /f /im explorer.exe && explorer.exe`)
4. Verify the registry entries exist

**Registration fails:**
1. Run Command Prompt as Administrator
2. Ensure the DLL file is not in use by other processes
3. Check Windows Event Log for detailed error messages

**Property page not loading:**
1. Check the Debug Output in Visual Studio for error messages
2. Verify the dialog resource (IDD_DIALOG1) exists
3. Ensure the property page callback functions are properly implemented

### Debug Output
The extension outputs debug information to the Visual Studio Output window when debugging. Look for messages prefixed with "FileTrail:" to track the extension's behavior.

## License

This project is licensed under the GNU Affero General Public License v3.0. See the [LICENSE.txt](LICENSE.txt) file for full license details.

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines
- Follow ATL/COM best practices
- Ensure proper reference counting for COM objects
- Test with multiple file selections and different file types
- Verify compatibility with different Windows versions
- Add appropriate debug output for troubleshooting

## Acknowledgments

This project builds upon Windows Shell Extension development patterns and ATL framework capabilities. Special thanks to the Windows development community for documentation and examples.

## Support

For issues, feature requests, or questions:
1. Check the [Issues](../../issues) page for existing discussions
2. Create a new issue with detailed information about your problem
3. Include Windows version, file types tested, and any error messages

---

**Note**: This shell extension modifies Windows Explorer behavior. Always test in a safe environment before deploying to production systems.