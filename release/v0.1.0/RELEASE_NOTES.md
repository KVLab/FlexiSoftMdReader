# FlexiSoftMdReader v0.1.0 Release Notes

## Release Information

- **Version**: v0.1.0
- **Release Date**: 2026-07-11
- **Status**: Initial Release

## Build Information

- **Platform**: Win32 (x86 - 32-bit)
- **Windows XP Compatible**: Yes ✓
- **Subsystem Version**: 5.01 (Windows XP)
- **Visual Studio Toolset**: v141_xp (Visual Studio 2015)
- **Runtime Library**: Static (/MT) – No external DLLs required
- **Executable Size**: ~450 KB

## Key Features

- ✓ Lightweight standalone Markdown viewer
- ✓ Windows XP, Vista, 7, 8, 10, 11 support
- ✓ Read-only Markdown file display
- ✓ Custom font support (system fonts or TTF files)
- ✓ Relative image path support (PNG, JPG, GIF, BMP)
- ✓ UTF-8 encoding support
- ✓ No external dependencies
- ✓ Integrated with FlexiSoft Runtime

## Supported Markdown Features

- Headings (#, ##, ###)
- Bullet and numbered lists
- Inline code and code blocks
- Bold and italic formatting
- Simple tables
- Images with relative paths
- Links (displayed as text)

## What's Included in Release Package

```
FlexiSoftMdReader_v0.1.0_win32_xp.zip
├── FlexiSoftMdReader.exe          (467 KB)
├── README.md                       (Main documentation)
├── examples/
│   ├── README.md
│   ├── markdown/
│   │   ├── example_basic.md
│   │   ├── example_tables.md
│   │   ├── example_images.md
│   │   └── assets/
│   │       ├── sample_image.png
│   │       └── warning_icon.png
│   └── usage/
│       ├── basic_usage.md
│       ├── flexisoft_runtime_usage.md
│       └── font_usage.md
└── docs/
    └── wiki/
        ├── Home.md
        ├── Build-Windows-XP.md
        ├── Usage.md
        ├── Markdown-Support.md
        ├── Integration-With-FlexiSoft-Runtime.md
        ├── Release-Process.md
        └── Troubleshooting.md
```

## Verification

### Checksum Verification

```batch
certutil -hashfile FlexiSoftMdReader.exe SHA256
certutil -hashfile FlexiSoftMdReader_v0.1.0_win32_xp.zip SHA256
```

**Expected SHA256**:
```
FlexiSoftMdReader.exe: 40962B9029BD68CD0EC18B08711E441B541DEE1350BCA86C3040195325B8876B
FlexiSoftMdReader_v0.1.0_win32_xp.zip: B96953030AFEFBBBCD8FA37C331CE73A4ADBD83432191A9577A04E32CEAEDB12
```

### Windows XP Compatibility Verification

```batch
dumpbin /headers FlexiSoftMdReader.exe | findstr /i "subsystem machine"
```

**Expected Output**:
```
  14C machine (x86)
  5.01 subsystem version
  2 subsystem (Windows GUI)
```

If subsystem version is not `5.01`, the build is not XP-compatible. Do not use.

## Installation Instructions

1. Extract `FlexiSoftMdReader_v0.1.0_win32_xp.zip`
2. Copy `FlexiSoftMdReader.exe` to your application folder
3. Copy examples and documentation to your project
4. Place Markdown files in a `docs/` folder
5. Test: `FlexiSoftMdReader.exe "docs\README.md"`

## Usage Examples

### Open Markdown with Default Font

```batch
FlexiSoftMdReader.exe "docs\README.md"
```

### Open with System Font

```batch
FlexiSoftMdReader.exe "docs\README.md" "Tahoma"
FlexiSoftMdReader.exe "docs\MANUAL_cz.md" "Arial"
```

### Open with Custom Font File

```batch
FlexiSoftMdReader.exe "docs\README.md" "fonts\NotoSans-Regular.ttf"
```

## System Requirements

- **OS**: Windows XP SP3 or later, Vista, 7, 8, 10, 11
- **Architecture**: x86 (32-bit)
- **RAM**: 10 MB (minimal)
- **Disk Space**: 500 KB
- **Dependencies**: None (built with static runtime)

## Supported Windows Versions

| OS | Support |
|---|---|
| Windows XP SP3 | ✓ Yes |
| Windows Vista | ✓ Yes |
| Windows 7 | ✓ Yes |
| Windows 8/8.1 | ✓ Yes |
| Windows 10 | ✓ Yes |
| Windows 11 | ✓ Yes |

## Known Limitations

1. **Simple Markdown viewer** – Not a full GitHub Flavored Markdown renderer
2. **Read-only** – Files cannot be edited
3. **Basic table support** – Tables rendered as preformatted text, not HTML
4. **No scripting** – No JavaScript or code execution
5. **Limited styling** – No CSS support, basic formatting only

These limitations are intentional for compatibility and simplicity.

## Integration with FlexiSoft Runtime

FlexiSoftMdReader is designed for use with FlexiSoft Runtime:

```batch
FlexiSoftMdReader.exe "docs\README_cz.md" "Tahoma"
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"
```

See `docs/wiki/Integration-With-FlexiSoft-Runtime.md` for details.

## Documentation

- **README.md** – Quick start and features
- **docs/wiki/Usage.md** – Command line usage
- **docs/wiki/Build-Windows-XP.md** – Building from source
- **docs/wiki/Markdown-Support.md** – Supported syntax
- **examples/** – Usage examples and Markdown samples
- **docs/wiki/Troubleshooting.md** – Common issues

## Future Versions

Potential improvements for future releases:

- [ ] Performance optimizations
- [ ] Additional Markdown features
- [ ] Localization support
- [ ] Configuration file support
- [ ] Enhanced table rendering

## Known Issues

None reported for v0.1.0. Please report issues on GitHub.

## Support and Feedback

- **GitHub Issues**: https://github.com/KVLab/FlexiSoftMdReader/issues
- **GitHub Discussions**: https://github.com/KVLab/FlexiSoftMdReader/discussions

## License

See LICENSE file in repository.

## Changelog

### v0.1.0 (2026-07-11)

- Initial release
- Windows XP compatibility (subsystem 5.01)
- Basic Markdown viewer implementation
- Font support (system and TTF files)
- Relative image path support
- Comprehensive documentation
- Example files and usage guides
- Built with Visual Studio 2015 XP toolset

---

**Thank you for using FlexiSoftMdReader!**

For questions or issues, please visit the GitHub repository:
https://github.com/KVLab/FlexiSoftMdReader
