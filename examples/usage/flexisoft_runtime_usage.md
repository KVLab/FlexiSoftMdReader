# FlexiSoft Runtime Integration

FlexiSoft Runtime is the primary user of FlexiSoftMdReader. This document describes how the runtime calls the viewer.

## Runtime Context

FlexiSoft Runtime runs on Windows systems, including older machines with Windows XP. When users need to view documentation files, the runtime launches FlexiSoftMdReader to display them.

## Calling FlexiSoftMdReader

The runtime passes two arguments to FlexiSoftMdReader:

1. **Document path** – Full or relative path to a `.md` file
2. **Font specification** – Either a system font name or a path to a `.ttf` font file

### Example Calls from Runtime

```batch
FlexiSoftMdReader.exe "docs\README.md" "Tahoma"
FlexiSoftMdReader.exe "docs\MANUAL_cz.md" "Tahoma"
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"
FlexiSoftMdReader.exe "docs\README_de.md" "Arial"
```

## Document Paths

The runtime passes absolute or relative paths to Markdown files in the installation folder:

```
FlexiSoft_runtime/
├── docs/
│   ├── README.md
│   ├── README_cz.md
│   ├── MANUAL_uk.md
│   └── MANUAL_de.md
├── FlexiSoftMdReader.exe
└── ...
```

When the viewer opens `docs/MANUAL_cz.md` and encounters image references like `![img](images/example.png)`, it resolves them relative to the document directory: `docs/images/example.png`.

## Font Argument

The runtime determines which font to use based on:
- User language/localization settings
- Available system fonts
- Optional font files packaged with the runtime

### Font Face Example

If the runtime uses the default system UI font:

```batch
FlexiSoftMdReader.exe "docs\MANUAL_cz.md" "Tahoma"
```

### Font File Example

If the runtime includes custom fonts:

```batch
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"
```

## Fallback Behavior

If the specified font cannot be loaded, FlexiSoftMdReader automatically uses a safe Windows system font (e.g., MS Sans Serif, Segoe UI, or default GUI font).

## Window Behavior

The viewer opens as a standalone window. The runtime does not wait for the window to close; users can continue using the runtime while documentation is displayed.

The window can be closed at any time. Closing it does not affect the runtime.

## XP Compatibility

FlexiSoftMdReader is built with Windows XP (subsystem 5.01) compatibility. This ensures it works on:
- Windows XP SP3
- Windows Vista and later
- All supported Windows versions

On Windows XP machines where modern .md viewers or Markdown file associations do not exist, FlexiSoftMdReader provides a reliable way to display documentation.

## Error Handling

If a document file is not found, FlexiSoftMdReader displays an error message and exits. The runtime is responsible for ensuring the document path is correct.
