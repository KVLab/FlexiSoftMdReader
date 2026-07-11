# Markdown Support

This page documents the Markdown syntax supported by FlexiSoftMdReader.

## Headings

Markdown headings using `#`:

```markdown
# Heading 1
## Heading 2
### Heading 3
```

Supported levels: **1, 2, 3**

(Deeper levels like `####` may be treated as regular text or level 3)

## Paragraphs

Paragraphs are separated by blank lines:

```markdown
This is a paragraph.

This is another paragraph.
```

Line breaks are preserved within paragraphs.

## Lists

### Bullet Lists

```markdown
- Item 1
- Item 2
- Item 3
```

Also supports `*` or `+`:

```markdown
* Item 1
+ Item 2
```

Nested lists:

```markdown
- Item 1
  - Sub-item 1.1
  - Sub-item 1.2
- Item 2
```

### Numbered Lists

```markdown
1. First item
2. Second item
3. Third item
```

Nested numbered and bullet lists:

```markdown
1. Main item
   - Detail A
   - Detail B
2. Another item
```

## Inline Formatting

### Bold

```markdown
**bold text**
```

Also supports `__bold text__`

### Italic

```markdown
*italic text*
```

Also supports `_italic text_`

### Bold and Italic

```markdown
***bold and italic***
```

### Inline Code

```markdown
`variable_name`
`function()`
`system.call()`
```

Inline code is rendered in a distinct font or style.

## Code Blocks

Fenced code blocks with triple backticks:

````markdown
```
plain text code block
```
````

With language hint (for formatting):

````markdown
```cpp
#include <windows.h>
int main() { return 0; }
```
````

Indented code (four spaces) may also be supported:

```
    code here
    indented
```

## Tables

Simple tables with pipe separators:

```markdown
| Column 1 | Column 2 |
|----------|----------|
| Cell 1   | Cell 2   |
| Cell 3   | Cell 4   |
```

**Note**: Tables are rendered as readable **preformatted text**, not as complex HTML tables. Alignment hints are ignored. Focus is on legibility and simplicity.

## Images

Markdown image syntax with relative paths:

```markdown
![alt text](path/to/image.png)
```

- **alt text** – Displayed if image fails to load
- **path/to/image.png** – Relative path from the Markdown file directory

### Supported Image Formats

- **PNG** (.png)
- **JPG/JPEG** (.jpg, .jpeg)
- **GIF** (.gif)
- **BMP** (.bmp)

### Path Resolution

Image paths are resolved **relative to the Markdown file directory**, not the executable location.

If you open `docs/README.md` and it contains `![](images/logo.png)`, the viewer looks for the image at `docs/images/logo.png`.

## Links

Markdown link syntax:

```markdown
[Link text](https://example.com)
[Local link](./README.md)
```

**Note**: Links are displayed as text. They are not clickable. This is intentional for a simple documentation viewer.

## Horizontal Rules

Supported in some implementations:

```markdown
---
***
___
```

## Blockquotes

Limited support:

```markdown
> This is a blockquote
> Continued on next line
```

Depending on implementation, blockquotes may be displayed as indented or plain text.

## What Is NOT Supported

- **HTML** – No raw HTML rendering
- **Subscript/Superscript** – `H~2~O` not supported
- **Task lists** – `- [x] Task` not fully supported
- **Strikethrough** – `~~text~~` may not render
- **Inline HTML** – No HTML tags in Markdown
- **Footnotes** – No footnote syntax
- **Emojis** – No emoji rendering
- **Advanced tables** – No complex table features (colspan, rowspan, etc.)

## Limitations and Design

FlexiSoftMdReader is designed for **simple documentation display**, not full Markdown standard compliance.

Goals:
- ✓ Display typical README and MANUAL files
- ✓ Show code examples
- ✓ Render images with relative paths
- ✓ Work on Windows XP and modern systems
- ✓ Minimal dependencies
- ✗ Full GitHub Flavored Markdown
- ✗ Complex formatting
- ✗ Dynamic content

For documentation that requires advanced Markdown features, consider using other tools.

## Character Encoding

- **UTF-8** (with or without BOM) is supported
- Non-ASCII characters are preserved
- File encoding is automatically detected
- Suitable for multilingual documentation (Czech, Ukrainian, German, French, etc.)

## File Behavior

- Files are opened **read-only**
- Files are **not modified**
- File associations are **not changed**
- No temporary files are created
