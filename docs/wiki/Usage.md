# Usage

FlexiSoftMdReader is a command-line Markdown viewer for Windows.

## Command Syntax

```
FlexiSoftMdReader.exe "<path-to-file.md>" [font-argument]
```

## Parameters

| Parameter | Required | Type | Example |
|-----------|----------|------|---------|
| File path | Yes | String | `"docs\README.md"` |
| Font | No | String | `"Tahoma"` or `"fonts\Custom.ttf"` |

## Basic Examples

### Open with Default Font

```batch
FlexiSoftMdReader.exe "docs\README.md"
```

Opens `docs/README.md` with the default Windows GUI font.

### Specify System Font

```batch
FlexiSoftMdReader.exe "docs\README.md" "Tahoma"
FlexiSoftMdReader.exe "docs\README.md" "Arial"
FlexiSoftMdReader.exe "docs\README.md" "Courier New"
```

Uses the named system font. If not available, falls back to default.

### Specify Font File

```batch
FlexiSoftMdReader.exe "docs\README.md" "fonts\NotoSans-Regular.ttf"
FlexiSoftMdReader.exe "docs\README.md" "C:\Fonts\Custom.ttf"
```

Loads a TrueType font from the given path. If the file doesn't exist, treats the argument as a font face name.

## Font Fallback

When a font is specified, the viewer tries:

1. **Font file** – Is it a valid `.ttf` file?
2. **Font face** – Is it a system font?
3. **Default** – Use Windows default GUI font

## Paths

### Relative Paths

```batch
FlexiSoftMdReader.exe "README.md"
FlexiSoftMdReader.exe "..\docs\README.md"
FlexiSoftMdReader.exe "docs\MANUAL.md"
```

### Absolute Paths

```batch
FlexiSoftMdReader.exe "C:\Users\Alice\Documents\README.md"
FlexiSoftMdReader.exe "\\server\share\docs\README.md"
```

### Paths with Spaces

Always quote paths that contain spaces:

```batch
FlexiSoftMdReader.exe "My Documents\README.md"
FlexiSoftMdReader.exe "C:\Program Files\MyApp\docs\README.md"
```

## Practical Examples

### Documentation Viewer Script

```batch
@echo off
REM Open project documentation
FlexiSoftMdReader.exe "%CD%\docs\README.md" "Tahoma"
```

### Multilingual Documentation

```batch
@echo off
REM Open docs in Czech with appropriate font
FlexiSoftMdReader.exe "docs\README_cz.md" "Tahoma"

REM Open docs in Ukrainian
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "Segoe UI"
```

### With Custom Font

```batch
@echo off
REM Use custom font file if available
IF EXIST "fonts\CustomFont.ttf" (
    FlexiSoftMdReader.exe "docs\README.md" "fonts\CustomFont.ttf"
) ELSE (
    FlexiSoftMdReader.exe "docs\README.md" "Tahoma"
)
```

### From Visual Studio

Create a batch file in your project:

```batch
@echo off
FlexiSoftMdReader.exe "%~dp0README.md"
```

Then call it from your project's pre/post-build events.

## Return Code

FlexiSoftMdReader exits with code `0` on successful display, non-zero on error (file not found, etc.).

In batch files:

```batch
FlexiSoftMdReader.exe "README.md"
IF %ERRORLEVEL% NEQ 0 (
    echo Error: Could not open README.md
    EXIT /B 1
)
```

## Image Resolution

When opening `docs/README.md`, relative image references like `![alt](images/logo.png)` are resolved relative to `docs/` directory:

```
docs/
├── README.md
└── images/
    └── logo.png
```

## File Encoding

FlexiSoftMdReader accepts:

- **UTF-8** (with or without BOM)
- **Text files** with Markdown content

Files are opened read-only and not modified.

## Supported Markdown Syntax

- Headings (#, ##, ###)
- Lists (bullet and numbered)
- Inline code and code blocks
- Bold and italic
- Tables (simple format)
- Images (with relative paths)
- Links (displayed as text)

See [Markdown-Support.md](Markdown-Support.md) for details.
