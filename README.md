# FlexiSoftMdReader

A lightweight, standalone Markdown documentation viewer for Windows, with Windows XP compatibility.

## Purpose

FlexiSoftMdReader is a small Win32 utility used by FlexiSoft Runtime to display Markdown documentation (.md files) in a simple GUI window. It is especially useful on Windows XP environments where .md file associations and modern viewers may not be available.

**Note:** This is a small utility viewer for basic Markdown display, not a full GitHub Markdown renderer. It supports the most common Markdown syntax needed for documentation.

## Features

- Lightweight standalone executable (no dependencies)
- Windows XP compatible (v5.01 subsystem)
- Read-only Markdown viewer with optional font customization
- Support for relative image paths
- UTF-8 encoding support (with or without BOM)
- Customizable fonts (TTF files or system fonts)

## Supported Markdown Syntax

- **Headings**: `#`, `##`, `###`
- **Lists**: Bullet lists (`-`, `*`) and numbered lists (`1.`, `2.`, etc.)
- **Code**: Inline code (backticks) and fenced code blocks (triple backticks)
- **Formatting**: Bold (`**text**`), italic (`*text*`)
- **Tables**: Simple tables displayed as readable preformatted text
- **Images**: `![alt text](relative/path.png)` (PNG, JPG/JPEG, GIF, BMP supported)
- **Links**: `[text](url)` displayed as text

## Usage

### Basic Usage

```
FlexiSoftMdReader.exe "<path-to-document.md>"
```

### With Custom Font

```
FlexiSoftMdReader.exe "<path-to-document.md>" "<font-file-or-font-face>"
```

### Examples

Open a Markdown file with default font:
```
FlexiSoftMdReader.exe "docs\README.md"
```

Open with a specific system font:
```
FlexiSoftMdReader.exe "docs\MANUAL_cz.md" "Tahoma"
```

Open with a TrueType font file:
```
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"
```

## Font Handling

The program supports three font modes:

1. **No font argument**: Uses Windows default GUI font
2. **Font file (.ttf)**: If the path exists and can be loaded as a TrueType font, it is used
3. **Font face name**: If no matching file is found, the name is treated as a system font face

If a font cannot be loaded, the application automatically falls back to a safe Windows system font.

## Image Support

Images are referenced with relative paths:

```markdown
![Logo](images/logo.png)
![Example](../shared/example.jpg)
```

Image paths are resolved relative to the Markdown file's directory, not the executable location.

Supported image formats: **PNG, JPG/JPEG, GIF, BMP**

## Build Requirements

- **Visual Studio** 2015 or later with C++ desktop workload
- **XP Toolset** v141_xp installed
- **Windows SDK** included with Visual Studio

### Build Configuration

- **Configuration**: Release
- **Platform**: Win32
- **Toolset**: v141_xp (Visual Studio 2015 with XP support)

## Building the Project

1. Open `FlexiSoftMdReader.sln` in Visual Studio
2. Select **Release | Win32** configuration
3. Right-click the project and select **Build** (or **Ctrl+B**)
4. The executable is created at: `FlexiSoftMdReader\Release\FlexiSoftMdReader.exe`

### Build Settings Reference

**Compiler (ClCompile):**
- Preprocessor: `WIN32;_WINDOWS;NDEBUG;UNICODE;_UNICODE;WINVER=0x0501;_WIN32_WINNT=0x0501`
- Runtime Library: `MultiThreaded (/MT)`
- Subsystem: Windows GUI

**Linker:**
- Subsystem: Windows
- Minimum Required Version: 5.01

## Verifying Windows XP Compatibility

Use the `dumpbin` tool to verify the executable targets Windows XP:

```
dumpbin /headers FlexiSoftMdReader.exe | findstr /i "subsystem machine"
```

Expected output:

```
  14C machine (x86)
  5.01 subsystem version
  2 subsystem (Windows GUI)
```

Key values:
- **14C**: x86 (32-bit)
- **5.01**: Windows XP subsystem version
- **2**: Windows GUI application

## Release Package

The Release build executable should be distributed as:

```
dist/FlexiSoftMdReader.exe
```

Or, if using GitHub Releases, upload the executable as a release asset rather than committing build artifacts into source folders.

### Release Contents

- `FlexiSoftMdReader.exe` - Main application (approx. 20-30 KB)
- No additional runtime libraries required (built with /MT - static runtime)
- No configuration files needed

## Repository Structure

```
FlexiSoftMdReader/
├── FlexiSoftMdReader/
│   ├── main.cpp                      # Entry point and window creation
│   ├── main.h                        # Main headers
│   ├── markdown.cpp                  # Markdown parsing logic
│   ├── markdown.h                    # Markdown parser interface
│   ├── html_view.cpp                 # HTML rendering in Windows
│   ├── html_view.h                   # HTML view interface
│   ├── FlexiSoftMdReader.vcxproj    # Visual Studio project file
│   └── FlexiSoftMdReader.vcxproj.filters
├── examples/
│   └── example.md                    # Example Markdown file
├── FlexiSoftMdReader.sln             # Visual Studio solution
├── README.md                         # This file
├── .gitignore                        # Git ignore rules
└── LICENSE                           # License information (if applicable)
```

## Notes

- The viewer is read-only; Markdown files are opened and displayed but not modified
- File encoding is automatically detected (UTF-8 with or without BOM)
- Post-build copy steps can be configured locally if needed; they are not part of the public repository
- This utility is optimized for simplicity and compatibility, not full Markdown standard compliance

## Support

For issues or suggestions related to FlexiSoftMdReader, please report them in the repository issue tracker.
