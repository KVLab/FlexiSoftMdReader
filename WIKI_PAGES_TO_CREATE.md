# GitHub Wiki Pages to Create

This file contains the content for GitHub Wiki pages that should be created manually.

**To create the wiki:**

1. Go to: https://github.com/KVLab/FlexiSoftMdReader
2. Click the **Wiki** tab
3. Click **Create the first page** or **New Page**
4. For each page below, create a new wiki page with the exact title and paste the content

---

## Page 1: Home

**Title**: Home

```markdown
# FlexiSoftMdReader Wiki

Welcome to the FlexiSoftMdReader wiki.

FlexiSoftMdReader is a lightweight standalone Markdown viewer for Windows XP and modern systems.

## Quick Links

- **[Usage](Usage)** – Command line usage examples
- **[Build Windows XP](Build-Windows-XP)** – How to build with XP support
- **[Markdown Support](Markdown-Support)** – Supported Markdown syntax
- **[Font Handling](Font-Handling)** – Font configuration
- **[FlexiSoft Runtime Integration](FlexiSoft-Runtime-Integration)** – Runtime integration guide
- **[Release Process](Release-Process)** – How to create releases
- **[Troubleshooting](Troubleshooting)** – Common issues and solutions

## Key Features

- Windows XP compatible (subsystem 5.01)
- Standalone executable (no .NET, no WebView2)
- Read-only Markdown viewer
- Font customization support
- Relative image paths

## Getting Started

1. Download from: https://github.com/KVLab/FlexiSoftMdReader/releases
2. Extract and copy `FlexiSoftMdReader.exe` to your application folder
3. Run: `FlexiSoftMdReader.exe "path\to\file.md"`

For detailed usage, see [Usage](Usage).

## Repository

- **GitHub**: https://github.com/KVLab/FlexiSoftMdReader
- **Source**: C++ Win32 application
- **License**: See repository for details
```

---

## Page 2: Usage

**Title**: Usage

```markdown
# Usage

FlexiSoftMdReader is a command-line tool for viewing Markdown files.

## Basic Command

```batch
FlexiSoftMdReader.exe "<path>" [font]
```

## Parameters

- `<path>` – Path to Markdown file (required)
- `[font]` – Optional font (system font name or `.ttf` file path)

## Examples

### Default Font

```batch
FlexiSoftMdReader.exe "docs\README.md"
```

### System Font

```batch
FlexiSoftMdReader.exe "docs\README.md" "Tahoma"
FlexiSoftMdReader.exe "docs\README.md" "Arial"
```

### Font File

```batch
FlexiSoftMdReader.exe "docs\README.md" "fonts\NotoSans.ttf"
```

## Paths

### Relative Paths

```batch
FlexiSoftMdReader.exe "..\docs\README.md"
FlexiSoftMdReader.exe ".\examples\markdown_basic.md"
```

### Absolute Paths

```batch
FlexiSoftMdReader.exe "C:\Users\Alice\docs\README.md"
```

### Paths with Spaces

Always quote paths with spaces:

```batch
FlexiSoftMdReader.exe "My Documents\README.md"
```

## From Batch Files

```batch
@echo off
FlexiSoftMdReader.exe "%CD%\docs\README.md" "Tahoma"
```

## Font Behavior

When a font is specified:

1. Try to load as TTF file
2. Try to find as system font
3. Fall back to Windows default

If a font cannot be loaded, the viewer automatically uses a safe system font.

## Image Resolution

Image paths in Markdown are resolved relative to the file's directory.

Example: If opening `docs/README.md` with `![](images/logo.png)`, the image is loaded from `docs/images/logo.png`.
```

---

## Page 3: Build Windows XP

**Title**: Build-Windows-XP

```markdown
# Building for Windows XP

FlexiSoftMdReader is built to be compatible with Windows XP (subsystem 5.01).

## Requirements

- **Visual Studio** 2015 or later
- **XP Toolset (v141_xp)** – Must be installed
- **Windows SDK** – Usually included with Visual Studio

## Installing XP Toolset

If not already installed:

1. Open **Visual Studio Installer**
2. Click **Modify**
3. Go to **Individual components**
4. Search for "XP"
5. Install:
   - MSVC v141 - VS 2015 C++ build tools (v14.1)
   - Windows XP Support for C++
6. Click **Modify** to install

## Build Steps

1. Open `FlexiSoftMdReader.sln` in Visual Studio
2. Select **Release | Win32** configuration
3. Press **Ctrl+B** or **Build > Build Solution**
4. Wait for completion

Output: `FlexiSoftMdReader\Release\FlexiSoftMdReader.exe`

## Build Settings

| Setting | Value | Purpose |
|---------|-------|---------|
| Platform Toolset | v141_xp | Windows XP support |
| Character Set | Unicode | Proper text encoding |
| Runtime | MultiThreaded (/MT) | Static linking, no DLL deps |
| Subsystem | Windows (GUI) | Graphical application |
| Minimum Version | 5.01 | XP subsystem version |
| Preprocessor | WINVER=0x0501; _WIN32_WINNT=0x0501 | XP API subset |

## Verifying XP Compatibility

After building, verify with:

```batch
dumpbin /headers FlexiSoftMdReader.exe | findstr /i "subsystem machine"
```

**Expected output**:

```
  14C machine (x86)
  5.01 subsystem version
  2 subsystem (Windows GUI)
```

If subsystem is not `5.01`, the build is not XP-compatible.

## Testing on Windows XP

1. Copy executable to Windows XP machine or VM
2. Create a test Markdown file
3. Run: `FlexiSoftMdReader.exe "test.md"`
4. Verify window opens and displays content

## Troubleshooting

### Error: "XP Toolset not found"

Install v141_xp via Visual Studio Installer (see Requirements).

### Error: "Windows SDK not found"

Reinstall Visual Studio and select Windows SDK during installation.

### Build succeeds but dumpbin shows wrong subsystem

Ensure you selected **Release | Win32**, not Debug or x64.

Rebuild: **Build > Clean Solution**, then **Ctrl+B**.
```

---

## Page 4: Markdown Support

**Title**: Markdown-Support

```markdown
# Markdown Support

FlexiSoftMdReader supports common Markdown syntax for documentation display.

## Supported Elements

### Headings

```markdown
# Level 1
## Level 2
### Level 3
```

Supported: Levels 1, 2, 3

### Lists

**Bullet lists:**

```markdown
- Item 1
- Item 2
- Item 3
```

**Numbered lists:**

```markdown
1. First
2. Second
3. Third
```

### Code

**Inline:**

```markdown
This is `code` in text.
```

**Blocks:**

````markdown
```cpp
#include <windows.h>
```
````

### Text Formatting

```markdown
**bold**
*italic*
***bold and italic***
```

### Tables

```markdown
| Column 1 | Column 2 |
|----------|----------|
| Cell A   | Cell B   |
```

Tables are displayed as readable preformatted text.

### Images

```markdown
![alt text](relative/path.png)
```

Paths are relative to the Markdown file directory.

**Supported formats**: PNG, JPG, GIF, BMP

### Links

```markdown
[text](url)
```

Links are displayed as text (not clickable).

## Not Supported

- HTML tags
- Subscript/Superscript (H~2~O)
- Strikethrough (~~text~~)
- Task lists (- [x] Task)
- Footnotes
- Emojis
- Complex table formatting

## Character Encoding

Accepts UTF-8 with or without BOM. Suitable for multilingual documentation (Czech, Ukrainian, German, French, etc.).

## Image Path Resolution

When opening `docs/README.md`:
- Reference `![](images/logo.png)`
- Resolves to `docs/images/logo.png`

Paths must use backslashes (`\`) on Windows.
```

---

## Page 5: Font Handling

**Title**: Font-Handling

```markdown
# Font Handling

FlexiSoftMdReader supports flexible font configuration.

## System Fonts

Specify any installed system font by name:

```batch
FlexiSoftMdReader.exe "file.md" "Tahoma"
FlexiSoftMdReader.exe "file.md" "Arial"
FlexiSoftMdReader.exe "file.md" "Courier New"
```

If the font exists on the system, it is used. Otherwise falls back to default.

## Font Files

Specify a path to a TrueType (.ttf) file:

```batch
FlexiSoftMdReader.exe "file.md" "fonts\CustomFont.ttf"
FlexiSoftMdReader.exe "file.md" "C:\Fonts\NotoSans-Regular.ttf"
```

If the file exists and can be loaded, it is used.

## Default Font

If no font argument is provided:

```batch
FlexiSoftMdReader.exe "file.md"
```

Uses Windows default GUI font (safest for compatibility).

## Fallback Behavior

When a font is specified, the viewer tries:

1. **Font file** – Is it a valid `.ttf`?
2. **Font face** – Is it a system font?
3. **Default** – Use Windows GUI font

This ensures the viewer always has a usable font.

## Unicode Support

For multilingual documentation, use Unicode fonts:

- **Tahoma** – Western and Cyrillic
- **Arial** – Wide Unicode support
- **Segoe UI** – Comprehensive Unicode
- **NotoSans** – Extensive multi-script support

## Relative vs. Absolute Paths

**Relative paths** (relative to current directory):
```batch
FlexiSoftMdReader.exe "file.md" "fonts\custom.ttf"
```

**Absolute paths**:
```batch
FlexiSoftMdReader.exe "file.md" "C:\Program Files\App\fonts\custom.ttf"
```

Both work. Relative paths are often more portable.

## Examples

```batch
# Default (always works)
FlexiSoftMdReader.exe "docs\README.md"

# System font (Czech documentation)
FlexiSoftMdReader.exe "docs\MANUAL_cz.md" "Tahoma"

# Custom font (Ukrainian documentation)
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"

# If font not found, falls back to default
FlexiSoftMdReader.exe "docs\README.md" "NonExistent.ttf"
```
```

---

## Page 6: FlexiSoft Runtime Integration

**Title**: FlexiSoft-Runtime-Integration

```markdown
# FlexiSoft Runtime Integration

FlexiSoftMdReader is designed for use with FlexiSoft Runtime.

## Runtime Context

FlexiSoft Runtime is a tool for managing machine fleets, especially legacy systems running Windows XP. When users need to view documentation, FlexiSoft Runtime launches FlexiSoftMdReader.

## How Runtime Calls the Viewer

Runtime passes document path and font:

```batch
FlexiSoftMdReader.exe "docs\README_cz.md" "Tahoma"
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"
```

## Parameters

1. **Document path** – Markdown file to display
2. **Font** – System font or TTF file path

## Window Behavior

- Opens as standalone window
- Runtime continues running (doesn't wait)
- User can keep documentation open while using runtime
- Multiple windows can be open at once
- Closing window doesn't affect runtime

## File Organization

Runtime documentation structure:

```
FlexiSoft_runtime/
├── docs/
│   ├── README.md
│   ├── README_cz.md
│   ├── MANUAL_uk.md
│   └── images/
├── FlexiSoftMdReader.exe
└── fonts/  (optional)
    └── *.ttf
```

## Image Paths

Images in Markdown are resolved relative to the document directory.

Example: `docs/README.md` with `![](images/logo.png)` looks for `docs/images/logo.png`.

## Font Selection

Runtime determines appropriate font based on:
- User language
- System fonts available
- Custom fonts packaged with runtime

## Error Handling

If document file cannot be found, FlexiSoftMdReader displays error. Runtime should:

1. Verify document path exists
2. Handle viewer exit codes
3. Provide user feedback

## XP Compatibility

FlexiSoftMdReader targets subsystem 5.01 (Windows XP). Works on:

- Windows XP SP3
- Windows Vista and later
- All modern Windows versions

On Windows XP where .md viewers don't exist, FlexiSoftMdReader provides reliable documentation display.
```

---

## Page 7: Release Process

**Title**: Release-Process

```markdown
# Release Process

How to create and publish releases of FlexiSoftMdReader.

## Pre-Release Checklist

- [ ] All code changes committed
- [ ] No uncommitted changes
- [ ] Code review completed
- [ ] Documentation updated

## Build Release

1. Open `FlexiSoftMdReader.sln`
2. Select **Release | Win32**
3. **Build** (Ctrl+B)
4. Verify build succeeds

## Verify XP Compatibility

```batch
dumpbin /headers FlexiSoftMdReader\Release\FlexiSoftMdReader.exe | findstr /i "subsystem machine"
```

**Must show**:
```
  14C machine (x86)
  5.01 subsystem version
```

If not, do not release.

## Test on Windows XP (Recommended)

1. Copy executable to Windows XP machine or VM
2. Test opening a Markdown file
3. Test font argument (system font)
4. Test font file (if available)
5. Verify everything works

## Copy Executable

```batch
copy FlexiSoftMdReader\Release\FlexiSoftMdReader.exe dist\
copy FlexiSoftMdReader\Release\FlexiSoftMdReader.exe release\v0.1.0\
```

## Create Distribution ZIP

Package `release/v0.1.0/FlexiSoftMdReader_v0.1.0_win32_xp.zip` with:

- `FlexiSoftMdReader.exe`
- `README.md`
- `examples/README.md`
- `examples/usage_*.md` (3 files)
- `examples/markdown_*.md` (3 files)
- `examples/assets/*.png`
- `release/v0.1.0/RELEASE_NOTES.md`

## Calculate Checksums

```batch
certutil -hashfile FlexiSoftMdReader.exe SHA256
certutil -hashfile FlexiSoftMdReader_v0.1.0_win32_xp.zip SHA256
```

Save to `release/v0.1.0/SHA256SUMS.txt`

## Git Commit

```batch
git add -A
git commit -m "Release v0.1.0: [description]"
git push origin master
```

## GitHub Release (Manual)

1. Go to https://github.com/KVLab/FlexiSoftMdReader/releases
2. Click **Create a new release**
3. **Tag**: `v0.1.0`
4. **Title**: `FlexiSoftMdReader v0.1.0`
5. **Description**: Copy from `RELEASE_NOTES.md`
6. **Upload files**:
   - `FlexiSoftMdReader.exe`
   - `FlexiSoftMdReader_v0.1.0_win32_xp.zip`
   - `SHA256SUMS.txt`
   - `RELEASE_NOTES.md`
7. Click **Publish release**

## Post-Release

- [ ] Verify GitHub Release published
- [ ] Download and test release package
- [ ] Verify checksums match
- [ ] Update documentation if needed
- [ ] Close any release-related issues

## Version Numbers

- `v0.1.0` – Initial release
- `v0.2.0` – New features
- `v0.1.1` – Bug fixes
- `v1.0.0` – Stable release

Use semantic versioning.
```

---

## Page 8: Troubleshooting

**Title**: Troubleshooting

```markdown
# Troubleshooting

Common issues and solutions.

## Application Won't Start

### "File not found"

**Solution**: 
- Verify path: `dir "path\to\file.md"`
- Use absolute paths for testing
- Check for typos

### No window appears

**Solution**:
- Try different Markdown file
- Test with simple text file
- Check Windows Event Viewer for errors

## Images Not Displaying

### Image shows broken

**Solution**:
1. Verify image path in Markdown
2. Check file exists: `dir "path\to\image.png"`
3. Use backslashes (`\`) not forward slashes
4. Paths are relative to document directory

### Unsupported format

**Solution**:
- Use PNG (most reliable)
- Supported: PNG, JPG, GIF, BMP
- Convert images to PNG

## Font Problems

### Font not loading

**Solution**:
1. Verify font file exists: `dir "fonts\font.ttf"`
2. Try without font argument (uses default)
3. Try common system font: `"Tahoma"`

### Wrong font displayed

**Solution**:
- Check font name spelling
- Use standard fonts: Tahoma, Arial, Courier New
- Check: Control Panel → Fonts
- Viewer falls back to default if font not found

### International characters appear as boxes

**Solution**:
- Use Unicode fonts: Tahoma, Arial, Segoe UI
- NotoSans for multilingual support
- Verify Markdown file uses UTF-8

## Windows XP Compatibility

### "Cannot run on this computer" (XP)

**Solution**:
1. Verify built with `v141_xp` toolset
2. Check subsystem: `dumpbin /headers FlexiSoftMdReader.exe`
3. Must show: `5.01 subsystem version`
4. Rebuild with correct toolset if needed

### Missing dependencies

**Solution**:
- Ensure build used static runtime (/MT)
- No external DLLs should be required
- Check: `dumpbin /dependents FlexiSoftMdReader.exe`
- Should only show system DLLs

## Markdown Issues

### Markdown not parsed correctly

**Solution**:
- Check [Markdown Support](Markdown-Support) for supported syntax
- Unsupported features won't render
- Use simple Markdown structures

### Tables appear as plain text

This is expected. Tables display as readable preformatted text.

## File Encoding

### Characters appear garbled

**Solution**:
1. Save file as UTF-8
2. In text editor: Save As → Encoding UTF-8
3. Verify in Notepad: Encoding should say "UTF-8"

## Getting Help

1. Check [Usage](Usage) for command syntax
2. Check [Markdown Support](Markdown-Support)
3. Review examples in repository
4. Search GitHub issues
5. Check [Font Handling](Font-Handling) for font problems
```

---

## Manual GitHub Wiki Creation Steps

1. Go to: https://github.com/KVLab/FlexiSoftMdReader
2. Click the **Wiki** tab
3. Click **Create the first page** 
4. Create **Home** page with content above
5. For each remaining page (Usage, Build-Windows-XP, etc.):
   - Click **New Page** (or **+** button)
   - Enter exact page title
   - Paste the content from above
   - Click **Save Page**

After creating all pages, the wiki will be accessible from the repository Wiki tab.
