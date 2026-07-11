# Build for Windows XP

This page explains how to build FlexiSoftMdReader with Windows XP compatibility.

## Requirements

- **Visual Studio** 2015 or later (any edition with C++ workload)
- **XP Toolset (v141_xp)** – Must be installed during Visual Studio setup
- **Windows SDK** – Usually included with Visual Studio

### Checking for XP Toolset

If you already have Visual Studio installed:

1. Open Visual Studio Installer
2. Click **Modify**
3. Go to **Individual components**
4. Search for "XP" or "v141"
5. Check **MSVC v141 - VS 2015 C++ build tools (v14.1)** and **Windows XP Support for C++**
6. Click **Modify** to install

## Building Release Configuration

1. Open `FlexiSoftMdReader.sln` in Visual Studio
2. Make sure the solution loads without errors
3. Select **Release | Win32** configuration from the dropdown
4. Press **Ctrl+B** or select **Build > Build Solution**
5. Wait for the build to complete

Output executable: `FlexiSoftMdReader\Release\FlexiSoftMdReader.exe`

## Build Settings (XP Compatible)

These settings are configured in `FlexiSoftMdReader.vcxproj`:

| Setting | Value | Reason |
|---------|-------|--------|
| Platform Toolset | v141_xp | Windows XP support |
| Character Set | Unicode | Proper encoding |
| Runtime Library | MultiThreaded (/MT) | Static linking, no DLL dependencies |
| Preprocessor | WINVER=0x0501; _WIN32_WINNT=0x0501 | XP API subset |
| Subsystem | Windows (GUI) | GUI application |
| Minimum Version | 5.01 | XP subsystem version |

## Verifying XP Compatibility

After building, verify that the executable targets Windows XP:

```
dumpbin /headers FlexiSoftMdReader.exe | findstr /i "subsystem machine"
```

**Expected output**:
```
  14C machine (x86)
  5.01 subsystem version
  2 subsystem (Windows GUI)
```

Key values:
- **14C** = x86 (32-bit processor)
- **5.01** = Windows XP subsystem version
- **2** = Windows GUI application

If the subsystem is not 5.01, the build is not XP-compatible.

## Common Build Issues

### Error: "XP Toolset Not Found"

**Solution**: Install the XP toolset via Visual Studio Installer (see Requirements section)

### Error: "MSVC v141 not found"

**Solution**: Ensure Visual Studio 2015 or compatible version is installed

### Error: "Windows SDK Not Found"

**Solution**: Reinstall Visual Studio and select Windows SDK in the installer

## Building Debug Configuration

For development and debugging:

1. Select **Debug | Win32** configuration
2. Press **Ctrl+B** to build
3. Output: `FlexiSoftMdReader\Debug\FlexiSoftMdReader.exe`

Debug builds include debugging symbols and are slightly larger.

## Post-Build Steps

After building:

1. Verify the executable with `dumpbin` (see above)
2. Test on Windows XP or use Windows XP VM
3. Copy to `dist/` folder for distribution
4. Update version in release package

## Release Build Process

After successful Release build:

```batch
copy FlexiSoftMdReader\Release\FlexiSoftMdReader.exe dist\
copy FlexiSoftMdReader\Release\FlexiSoftMdReader.exe release\v0.1.0\
```

See [Release-Process.md](Release-Process.md) for full release workflow.

## Troubleshooting Build Failures

1. **Clean first**: Select **Build > Clean Solution**, then rebuild
2. **Check paths**: Ensure project paths don't contain special characters
3. **Update Visual Studio**: Install latest updates
4. **Rebuild dependencies**: Manually rebuild `.h` files if needed
5. **Check disk space**: Ensure sufficient disk space for build outputs

## Testing on Windows XP

To test on Windows XP:

1. Copy `FlexiSoftMdReader.exe` to a Windows XP machine
2. Place a Markdown file in the same directory
3. Run: `FlexiSoftMdReader.exe "test.md"`
4. Verify the window opens and displays Markdown

Alternatively, use a Windows XP virtual machine for testing.
