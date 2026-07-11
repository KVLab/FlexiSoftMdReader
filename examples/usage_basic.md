# Basic Usage

FlexiSoftMdReader is a command-line Markdown viewer.

## Syntax

```batch
FlexiSoftMdReader.exe "<path-to-file.md>" [font-argument]
```

## Basic Examples

Open with default font:

```batch
FlexiSoftMdReader.exe "docs\README.md"
```

## Paths with Spaces

Quote paths that contain spaces:

```batch
FlexiSoftMdReader.exe "My Documents\README.md"
FlexiSoftMdReader.exe "C:\Program Files\MyApp\docs\MANUAL.md"
```

## Relative Paths

```batch
FlexiSoftMdReader.exe "..\docs\README.md"
FlexiSoftMdReader.exe ".\examples\markdown_basic.md"
```

## Absolute Paths

```batch
FlexiSoftMdReader.exe "C:\Users\Alice\docs\README.md"
FlexiSoftMdReader.exe "\\server\share\docs\README.md"
```

## From Batch Files

```batch
@echo off
REM Open documentation
FlexiSoftMdReader.exe "%CD%\docs\README.md"
```

## Supported File Extensions

While designed for `.md` files, any text file with Markdown content can be opened.
