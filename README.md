# FlexiSoftMdReader

A small standalone Win32 Markdown viewer for local project documentation.

## What It Is

FlexiSoftMdReader opens and displays Markdown (.md) files in a simple graphical window. It is designed for compatibility with older machines, including Windows XP systems.

## Why It Exists

Used primarily with FlexiSoft Runtime to display documentation reliably on old PCs where Markdown file associations and modern viewers do not exist. Solves the problem of viewing Markdown files on Windows XP and other legacy systems.

This is **not** a full GitHub Markdown renderer—it is a simple local documentation viewer.

## Quick Usage

```batch
FlexiSoftMdReader.exe "docs\README.md"
FlexiSoftMdReader.exe "docs\MANUAL_cz.md" "Tahoma"
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"
FlexiSoftMdReader.exe "examples\markdown_images.md"
```

## Supported Markdown

- **Headings**: `#`, `##`, `###`
- **Lists**: Bullet (`-`, `*`) and numbered (`1.`, `2.`, etc.)
- **Code**: Inline (backticks) and fenced blocks
- **Formatting**: Bold (`**text**`), italic (`*text*`)
- **Tables**: Rendered as readable preformatted text
- **Images**: `![alt](relative/path.png)` with relative path support
- **Links**: Displayed as text

**Supported image formats**: PNG, JPG/JPEG, GIF, BMP

## Font Argument

Optional second argument specifies font:

- **System font**: `FlexiSoftMdReader.exe "file.md" "Tahoma"`
- **Font file**: `FlexiSoftMdReader.exe "file.md" "fonts\Custom.ttf"`
- **Default**: If not provided or font not found, uses Windows default

Fallback: If font cannot be loaded, automatically uses a safe system font.

## Image Paths

Images use relative paths from the Markdown file directory:

```markdown
![alt](images/logo.png)
```

If Markdown file is `docs/README.md`, image path `![](images/logo.png)` resolves to `docs/images/logo.png`.

## File Behavior

- Opens files **read-only** (files not modified)
- Accepts **UTF-8** with or without BOM
- Does **not** change file associations
- **No dependencies**: No .NET, no WebView2, no Qt, no MFC

## Building

### Requirements

- Visual Studio 2015 or later (any C++ edition)
- **XP Toolset v141_xp** (must be installed)
- Windows SDK

### Build Steps

1. Open `FlexiSoftMdReader.sln`
2. Select **Release | Win32**
3. **Build** (Ctrl+B)
4. Output: `FlexiSoftMdReader\Release\FlexiSoftMdReader.exe`

### Build Settings (Windows XP Compatible)

| Setting | Value |
|---------|-------|
| Toolset | v141_xp |
| Platform | Win32 (x86) |
| Runtime | MultiThreaded (/MT) |
| Subsystem | Windows GUI |
| Min Version | 5.01 |
| Preprocessor | `WINVER=0x0501;_WIN32_WINNT=0x0501` |

### Verify XP Compatibility

```batch
dumpbin /headers FlexiSoftMdReader.exe | findstr /i "subsystem machine"
```

**Expected output**:
```
  14C machine (x86)
  5.01 subsystem version
  2 subsystem (Windows GUI)
```

## Release

Latest executable is tracked in:
- `dist/FlexiSoftMdReader.exe` – Clean final build

Release package in:
- `release/v0.1.0/FlexiSoftMdReader.exe`
- `release/v0.1.0/FlexiSoftMdReader_v0.1.0_win32_xp.zip`
- `release/v0.1.0/SHA256SUMS.txt`
- `release/v0.1.0/RELEASE_NOTES.md`

### GitHub Release (Manual)

To publish a GitHub Release:

1. Go to https://github.com/KVLab/FlexiSoftMdReader/releases
2. Click **Create a new release**
3. **Tag**: `v0.1.0`
4. **Title**: FlexiSoftMdReader v0.1.0
5. **Upload files**:
   - `release/v0.1.0/FlexiSoftMdReader.exe`
   - `release/v0.1.0/FlexiSoftMdReader_v0.1.0_win32_xp.zip`
   - `release/v0.1.0/SHA256SUMS.txt`
   - `release/v0.1.0/RELEASE_NOTES.md`
6. **Publish**

## Examples

See `examples/` folder for:
- Usage examples (`usage_*.md`)
- Markdown syntax samples (`markdown_*.md`)
- Sample images (`assets/`)

Run examples:
```batch
FlexiSoftMdReader.exe "examples\markdown_basic.md"
FlexiSoftMdReader.exe "examples\markdown_images.md"
```

## Documentation

- **GitHub Wiki**: Available after creation (see Wiki tab)
- **Examples**: `examples/README.md` and sample files
- **Release info**: `release/v0.1.0/RELEASE_NOTES.md`

## Limitations

- Simple Markdown viewer, not full GitHub Flavored Markdown
- Tables rendered as preformatted text, not HTML
- Read-only display, no file editing
- No script execution or dynamic content
- No CSS or complex styling

Intentional by design for simplicity and compatibility.

## License

See LICENSE file for details.

---

**Repository**: https://github.com/KVLab/FlexiSoftMdReader

Used with FlexiSoft Runtime for documentation display on Windows XP and modern systems.

