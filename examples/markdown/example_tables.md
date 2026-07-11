# Table Example

This file demonstrates table rendering in FlexiSoftMdReader.

## Simple Table

| Feature | Support | Notes |
|---------|---------|-------|
| Headings | Yes | Levels 1-3 |
| Lists | Yes | Bullet and numbered |
| Code | Yes | Inline and blocks |
| Images | Yes | PNG, JPG, GIF, BMP |
| Tables | Yes | Basic text format |
| Bold | Yes | `**text**` |
| Italic | Yes | `*text*` |
| Links | Yes | Displayed as text |

## Comparison Table

| Aspect | Windows XP | Windows 10+ |
|--------|-----------|------------|
| FlexiSoftMdReader | Works (subsystem 5.01) | Works |
| Modern browsers | Limited | Full support |
| .md file viewer | Rare | Common |
| Dependencies | None | Often required |

## Status Overview

| Item | Status | Completed |
|------|--------|-----------|
| Basic build | ✓ Complete | Yes |
| XP compatibility | ✓ Complete | Yes |
| Documentation | ✓ Complete | Yes |
| Examples | ✓ Complete | Yes |
| Font support | ✓ Complete | Yes |

## Technical Specification

| Property | Value | Details |
|----------|-------|---------|
| Platform | Win32 | x86 architecture |
| Subsystem | Windows GUI | Graphical application |
| Minimum OS | Windows XP | Version 5.01 |
| Runtime | Static (/MT) | No external DLLs needed |
| Encoding | UTF-8 | With or without BOM |

## Table Notes

Tables in FlexiSoftMdReader are rendered as **readable preformatted text**, not as a complex HTML table engine. This means:

- Alignment hints in Markdown are ignored
- Cell content wraps based on window width
- Simple borders or separators are displayed
- Content remains legible and scannable

For complex table layouts, consider using other tools. FlexiSoftMdReader is optimized for **documentation display**, not spreadsheet-like tables.
