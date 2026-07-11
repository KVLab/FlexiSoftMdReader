# Font Usage

FlexiSoftMdReader supports two ways to specify fonts: system fonts and font files.

## System Font (Font Face)

Use any installed system font:

```batch
FlexiSoftMdReader.exe "docs\README.md" "Tahoma"
FlexiSoftMdReader.exe "docs\README.md" "Arial"
FlexiSoftMdReader.exe "docs\README.md" "Courier New"
```

If the font exists, it is used. Otherwise falls back to default.

## Font File (TTF)

Specify a path to a TrueType file:

```batch
FlexiSoftMdReader.exe "docs\README.md" "fonts\CustomFont.ttf"
FlexiSoftMdReader.exe "docs\README.md" "C:\Fonts\NotoSans-Regular.ttf"
```

If the file exists and can be loaded, it is used.

## Fallback Chain

When a font argument is provided, the viewer tries:

1. **Is it a font file?** Does the path exist and load as TTF?
2. **Is it a font face?** Is it installed on the system?
3. **Default** – Use Windows default GUI font

This ensures the viewer always has a usable font.

## No Font Argument

If no second argument is provided, the default GUI font is used:

```batch
FlexiSoftMdReader.exe "docs\README.md"
```

## Examples

Default font:
```batch
FlexiSoftMdReader.exe "docs\README.md"
```

System font:
```batch
FlexiSoftMdReader.exe "docs\MANUAL_cz.md" "Tahoma"
```

Custom font file:
```batch
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"
```

## Unicode Support

For multilingual documentation (Czech, Ukrainian, German, etc.), use Unicode fonts like Tahoma, Arial, or Segoe UI.

NotoSans supports extensive Unicode for all scripts.
