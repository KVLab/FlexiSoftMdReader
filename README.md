# FlexiSoftMdReader

FlexiSoftMdReader is a small standalone Win32 Markdown viewer for local project documentation.

It was created for **FlexiSoft Runtime** and for older industrial PCs where Markdown files cannot be opened reliably by the operating system. The main target is simple documentation display on Windows XP and newer Windows systems.

## Features

* opens local `.md` files in a simple read-only window,
* supports UTF-8 Markdown files,
* supports relative image paths,
* accepts an optional font name or font file as a second argument,
* does not change file associations,
* does not need .NET, WebView2, Qt or MFC,
* can be built as a small Win32 application.

## Usage

```bat
FlexiSoftMdReader.exe "docs\\README.md"
FlexiSoftMdReader.exe "docs\\MANUAL_cz.md" "Tahoma"
FlexiSoftMdReader.exe "docs\\MANUAL_uk.md" "fonts\\NotoSans-Regular.ttf"
```

The first argument is the Markdown file.

The optional second argument can be either a system font name or a font file path.

## Supported Markdown

FlexiSoftMdReader is not a full GitHub Markdown renderer. It supports the subset needed for local service documentation.

More detailed build and release notes are in the project Wiki.
