# FlexiSoftMdReader

A small standalone Win32 Markdown viewer for local project documentation.

## Purpose

FlexiSoftMdReader opens `.md` files and displays their content in a simple graphical window. It is designed to work reliably on older machines, including Windows XP systems where Markdown file associations and modern viewers may not exist.

Primary use: **opening documentation from FlexiSoft Runtime on Windows XP and other legacy systems**.

This is a lightweight utility viewer—not a full GitHub Markdown renderer. It supports common Markdown syntax sufficient for practical documentation display.

## Features

- Standalone Win32 executable (no dependencies, no .NET, no WebView2)
- Windows XP compatible
- Read-only Markdown viewer with customizable fonts
- Relative image support (PNG, JPG, GIF, BMP)
- UTF-8 file encoding support
- Simple, fast rendering

## Quick Usage

```
FlexiSoftMdReader.exe "docs\README.md"
FlexiSoftMdReader.exe "docs\MANUAL_cz.md" "Tahoma"
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"
FlexiSoftMdReader.exe "examples\markdown\example_images.md"
```

## Supported Markdown

- **Headings**: `#`, `##`, `###`
- **Lists**: Bullet lists (`-`, `*`) and numbered lists (`1.`, `2.`, etc.)
- **Code**: Inline code and fenced code blocks
- **Formatting**: Bold (`**text**`), italic (`*text*`)
- **Tables**: Simple tables displayed as readable text
- **Images**: `![alt](relative/path.png)` with relative path resolution
- **Links**: `[text](url)` displayed as text

**Supported image formats**: PNG, JPG/JPEG, GIF, BMP

## Font Argument

The optional second argument specifies a font for the viewer window:

- **Font file** (`.ttf`): If the path exists and can be loaded as TrueType, it is used
- **Font face name** (e.g., `"Tahoma"`): Uses the named system font
- **Not provided**: Uses Windows default GUI font
- **Fallback**: If a font cannot be loaded, reverts to a safe Windows font

## Image Paths

Images are referenced with relative paths:

```markdown
![Logo](images/logo.png)
![Example](../shared/example.jpg)
```

Image paths are resolved relative to the Markdown file's directory.

## File Behavior

- Opens files in **read-only mode**
- Accepts **UTF-8 with or without BOM**
- **Does not modify** files
- **Does not change** file associations
- Resolves relative image paths from the Markdown file directory

## Building

### Requirements

- Visual Studio 2015 or later with C++ desktop workload
- XP Toolset: `v141_xp` must be installed
- Windows SDK

### Release Build

1. Open `FlexiSoftMdReader.sln` in Visual Studio
2. Select **Release | Win32** configuration
3. **Build** (Ctrl+B)
4. Output: `FlexiSoftMdReader\Release\FlexiSoftMdReader.exe`

### Build Settings (Documented)

| Setting | Value |
|---------|-------|
| Configuration | Release |
| Platform | Win32 |
| Toolset | v141_xp |
| Runtime | MultiThreaded (/MT) |
| Subsystem | Windows GUI |
| Minimum Version | 5.01 |
| Preprocessor | `WINVER=0x0501;_WIN32_WINNT=0x0501` |

### Verifying Windows XP Compatibility

```
dumpbin /headers FlexiSoftMdReader.exe | findstr /i "subsystem machine"
```

**Expected output**:
```
  14C machine (x86)
  5.01 subsystem version
  2 subsystem (Windows GUI)
```

If the subsystem version is not `5.01`, the build is not XP-compatible.

## Release Files

- **dist/FlexiSoftMdReader.exe** – Tracked clean final executable
- **release/v0.1.0/** – Release package folder with:
  - `FlexiSoftMdReader.exe` – Final executable
  - `FlexiSoftMdReader_v0.1.0_win32_xp.zip` – Distribution package
  - `SHA256SUMS.txt` – Checksum verification
  - `RELEASE_NOTES.md` – Release information

## Examples

See [examples/README.md](examples/README.md) for usage examples and sample Markdown files.

## Documentation

Additional documentation and wiki pages are in [docs/wiki/](docs/wiki/).

## Limitations

- Not a full Markdown standard implementation
- Table rendering is basic (readable text, not full HTML tables)
- No script execution or dynamic content
- Limited CSS/styling support
- Designed for documentation display, not complex formatting

## License

See LICENSE file for details.

---

**GitHub Repository**: https://github.com/KVLab/FlexiSoftMdReader

