# Font Usage

FlexiSoftMdReader supports two ways to specify fonts: system font faces and external font files.

## Font Face

A font face is a name of a font installed on the system. Examples:

- `Tahoma`
- `Arial`
- `Courier New`
- `Times New Roman`
- `Segoe UI`
- `MS Sans Serif`

### Using a System Font

```batch
FlexiSoftMdReader.exe "docs\README.md" "Tahoma"
```

If the font exists on the system, it is used. If not, the viewer falls back to the default GUI font.

## Font File

A font file is a path to a TrueType (`.ttf`) file. Examples:

```batch
FlexiSoftMdReader.exe "docs\README.md" "fonts\CustomFont.ttf"
FlexiSoftMdReader.exe "docs\README.md" "C:\Fonts\NotoSans-Regular.ttf"
```

The viewer first checks if the path is a valid TrueType font file:
1. If the file exists and can be loaded as a font, it is used
2. If the file doesn't exist or can't be loaded, the viewer treats the argument as a font face name
3. If the font face isn't found, it falls back to default

## Fallback Chain

When a font is specified, the viewer tries:

1. **Font file** – Is it a valid `.ttf` file?
2. **Font face** – Is it a system font?
3. **Default** – Use Windows default GUI font

This ensures the viewer always has a usable font.

## Relative vs. Absolute Paths

Font file paths can be relative or absolute:

**Relative paths** (relative to the current working directory):
```batch
FlexiSoftMdReader.exe "docs\README.md" "fonts\CustomFont.ttf"
```

**Absolute paths**:
```batch
FlexiSoftMdReader.exe "docs\README.md" "C:\Program Files\MyApp\fonts\CustomFont.ttf"
```

## No Font Argument

If no second argument is provided, the viewer uses the Windows default GUI font:

```batch
FlexiSoftMdReader.exe "docs\README.md"
```

This is the safest option for maximum compatibility.

## Examples

**Default font:**
```batch
FlexiSoftMdReader.exe "docs\README.md"
```

**System font (Tahoma):**
```batch
FlexiSoftMdReader.exe "docs\MANUAL_cz.md" "Tahoma"
```

**Custom font file:**
```batch
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"
```

**System font fallback:**
```batch
FlexiSoftMdReader.exe "docs\README.md" "NonExistentFont.ttf"
```
(If `NonExistentFont.ttf` doesn't exist, the viewer treats it as a face name and falls back to default if not found)

## Font and Localization

For international documentation (Czech, Ukrainian, German, French), ensure the font supports the required character set. Unicode fonts like Tahoma, Arial, or Segoe UI are safe choices.

Custom fonts like NotoSans support extensive Unicode coverage for multilingual documents.
