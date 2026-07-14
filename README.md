# FlexiSoftMdReader

FlexiSoftMdReader is a small standalone Win32 Markdown viewer for local documentation.

It is intended for situations where a simple local `.md` viewer is more practical than relying on file associations, a browser, an editor, or modern Windows components. Typical use cases are service documentation, machine documentation, offline documentation packages, and older industrial PCs.

## Features

* opens local Markdown files,
* displays documents read-only,
* supports UTF-8 text,
* supports relative image paths,
* supports an optional font argument,
* supports system font names and TrueType font files,
* does not modify opened files,
* does not change Windows file associations,
* does not require .NET, WebView2, Qt or MFC.

## Quick start

Open one of the included examples:

```bat
FlexiSoftMdReader.exe "examples\\markdown_basic.md"
FlexiSoftMdReader.exe "examples\\markdown_tables.md"
FlexiSoftMdReader.exe "examples\\markdown_images.md"
```

Open a document with a system font:

```bat
FlexiSoftMdReader.exe "examples\\markdown_basic.md" "Tahoma"
```

Open a document with a bundled font file:

```bat
FlexiSoftMdReader.exe "examples\\markdown_basic.md" "examples\\fonts\\NotoSans-Regular.ttf"
```

More detailed usage notes and examples are available in the project Wiki.
