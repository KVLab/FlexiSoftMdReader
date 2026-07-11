# Integration with FlexiSoft Runtime

FlexiSoftMdReader is designed primarily for use with FlexiSoft Runtime.

## Purpose in Runtime

FlexiSoft Runtime is a maintenance and configuration tool for machine fleets, especially legacy systems running Windows XP. When runtime users need to view documentation, they click a "View Documentation" button or menu item, which launches FlexiSoftMdReader.

The viewer opens reliably on machines where:
- Modern .md file associations do not exist
- Markdown viewers are not installed
- WebView2 or .NET are not available
- Windows XP is the OS

## How Runtime Calls FlexiSoftMdReader

The runtime passes the document path and an appropriate font:

```batch
FlexiSoftMdReader.exe "docs\README.md" "Tahoma"
FlexiSoftMdReader.exe "docs\MANUAL_cz.md" "Tahoma"
FlexiSoftMdReader.exe "docs\MANUAL_uk.md" "fonts\NotoSans-Regular.ttf"
```

### Parameters

1. **Document path** – Markdown file to display (relative or absolute)
2. **Font** – System font name or path to `.ttf` file

### Font Selection Logic

The runtime determines which font to use:

- **Default UI font** – For most documentation (e.g., `"Tahoma"`)
- **Localized font** – For language-specific documentation (e.g., Ukrainian)
- **Custom font file** – If the runtime includes custom fonts (e.g., `"fonts\NotoSans.ttf"`)

## Documentation Organization in Runtime

FlexiSoft Runtime includes documentation in multiple languages:

```
FlexiSoft_runtime/
├── docs/
│   ├── README.md
│   ├── README_cz.md      # Czech
│   ├── README_de.md      # German
│   ├── README_fr.md      # French
│   ├── MANUAL.md         # English
│   ├── MANUAL_cz.md      # Czech
│   ├── MANUAL_uk.md      # Ukrainian
│   ├── MANUAL_de.md      # German
│   ├── MANUAL_fr.md      # French
│   ├── images/
│   │   └── *.png
│   └── fonts/            (optional custom fonts)
│       └── *.ttf
├── FlexiSoftMdReader.exe
└── ...
```

## Image and Font Paths

### Images

When opening `docs/MANUAL_cz.md`, image references like `![](images/screenshot.png)` are resolved relative to the `docs/` directory.

### Fonts

When the runtime specifies `"fonts\NotoSans.ttf"`, the viewer looks for the file relative to the current working directory.

Both paths can be relative to the runtime's installation folder.

## Error Handling

If a document file cannot be opened, FlexiSoftMdReader displays an error message. The runtime should:

1. Verify the document path before calling the viewer
2. Handle viewer exit codes (0 = success, non-zero = error)
3. Provide user feedback if the document cannot be displayed

## Window Behavior

- The viewer opens as a **standalone window**
- The runtime **does not wait** for the window to close
- The window runs **independently** of the runtime
- Closing the viewer **does not affect** the runtime

Users can keep the documentation window open while using the runtime.

## XP Compatibility Context

On Windows XP machines:

- File associations for `.md` files typically do not exist
- Modern viewers are not installed or not compatible
- .NET Framework may not be available
- WebView2 may not be available

FlexiSoftMdReader solves this by providing:
- A **standalone** executable (no dependencies)
- **XP subsystem 5.01** compatibility
- **Static runtime** linking (no external DLLs)
- **Minimal API** usage (core Windows GUI only)

## Security and Sandboxing

- FlexiSoftMdReader opens files **read-only**
- Files are **not modified** or cached
- No **network access**
- No **external scripts** or code execution
- Files are displayed as-is

The runtime can safely launch the viewer without additional security concerns.

## Localization

FlexiSoftMdReader is language-neutral:

- Viewer UI uses Windows system language
- Markdown content is displayed as-is (supports UTF-8)
- Fonts are font-independent (work with any Unicode font)
- No hardcoded language-specific strings

This makes it suitable for:
- Czech documentation
- Ukrainian documentation
- German and French documentation
- Any language using Latin, Cyrillic, or other Unicode scripts

## Deployment with Runtime

FlexiSoftMdReader should be packaged with FlexiSoft Runtime:

1. Place `FlexiSoftMdReader.exe` in the runtime's installation folder
2. Place documentation in `docs/` subfolder
3. Place fonts (if any) in `docs/fonts/` subfolder
4. Call the viewer with relative paths (e.g., `"docs\README_cz.md"`)

## Version and Updates

- Track FlexiSoftMdReader version (current: v0.1.0)
- Update the viewer when deploying new runtime versions
- Ensure XP compatibility is maintained in updates
- Document changes in release notes

See [Release-Process.md](../wiki/Release-Process.md) for release workflow.
