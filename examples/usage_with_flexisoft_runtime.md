# FlexiSoft Runtime Integration

FlexiSoft Runtime uses FlexiSoftMdReader to display documentation.

## Runtime Calls

The runtime passes document path and font:

```batch
FlexiSoftMdReader.exe "docs\README_cz.md" "Tahoma"
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"
FlexiSoftMdReader.exe "docs\README_de.md" "Arial"
```

## Parameters

1. **Document path** – Markdown file to display
2. **Font** – System font name or path to `.ttf` file

## Context

FlexiSoft Runtime is a tool for old machines (including Windows XP) where Markdown viewers do not exist. FlexiSoftMdReader provides a reliable way to display documentation.

## Behavior

- Viewer opens as standalone window
- Runtime continues normally
- Window can be closed at any time
- Multiple documentation windows can be open simultaneously

## XP Compatibility

FlexiSoftMdReader is built for Windows XP (subsystem 5.01) and works on all modern Windows versions.

On Windows XP machines where `.md` file associations don't exist, FlexiSoftMdReader is the only reliable way to view Markdown documentation.
