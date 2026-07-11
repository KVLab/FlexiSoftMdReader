# Basic Usage

FlexiSoftMdReader is a command-line Markdown viewer. Run it from the command line, batch file, or any script.

## Basic Command

Open a Markdown file with default font:

```batch
FlexiSoftMdReader.exe "docs\README.md"
```

## Paths with Spaces

Paths with spaces must be quoted:

```batch
FlexiSoftMdReader.exe "My Documents\README.md"
FlexiSoftMdReader.exe "C:\Program Files\MyApp\docs\MANUAL.md"
```

Relative paths work:

```batch
FlexiSoftMdReader.exe "..\shared\README.md"
FlexiSoftMdReader.exe ".\examples\example_basic.md"
```

## Using System Fonts

Specify a system font as the second argument:

```batch
FlexiSoftMdReader.exe "docs\README.md" "Tahoma"
FlexiSoftMdReader.exe "docs\MANUAL.md" "Courier New"
FlexiSoftMdReader.exe "docs\MANUAL.md" "Arial"
```

The viewer will use the named font if available on the system. If not found, it falls back to the default GUI font.

## Using Font Files

Pass a `.ttf` file path:

```batch
FlexiSoftMdReader.exe "docs\README.md" "fonts\NotoSans-Regular.ttf"
FlexiSoftMdReader.exe "docs\MANUAL.md" "C:\Fonts\CustomFont.ttf"
```

If the file exists and can be loaded as a TrueType font, it is used. Otherwise, the viewer falls back to default.

## From Batch Files

Use in batch files to open documentation:

```batch
@echo off
REM Open the README with default font
FlexiSoftMdReader.exe "%CD%\docs\README.md"
```

Or with a specific font:

```batch
@echo off
REM Open MANUAL in Czech with Tahoma
FlexiSoftMdReader.exe "%CD%\docs\MANUAL_cz.md" "Tahoma"
```

## Exit Behavior

The viewer window is standalone. Close it to return to the command prompt or script.

## Supported File Extensions

While typically used with `.md` files, FlexiSoftMdReader can open any text file with Markdown content.
