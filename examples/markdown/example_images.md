# Image Example

This file demonstrates image support in FlexiSoftMdReader.

## Relative Image Paths

Images are referenced using relative paths from the Markdown file's directory:

```markdown
![alt text](assets/image.png)
```

When you open `examples/markdown/example_images.md`, the viewer looks for images in `examples/markdown/assets/`.

## Image Syntax

Standard Markdown image syntax:

```
![alt text](path/to/image.ext)
```

- `alt text` – Description displayed if image fails to load
- `path/to/image.ext` – Relative path to the image file

## Supported Image Formats

- **PNG** (.png) – Recommended, supports transparency
- **JPG/JPEG** (.jpg, .jpeg) – Lossy compression
- **GIF** (.gif) – Supports animation
- **BMP** (.bmp) – Windows bitmap format

## Example Images

Here are example images from the assets folder:

### Sample Image

![Sample image](assets/sample_image.png)

This is a placeholder image demonstrating relative path resolution.

### Warning Icon

![Warning icon](assets/warning_icon.png)

This is another example image.

## Path Resolution

Image paths are resolved **relative to the Markdown file directory**, not the executable:

```
Project/
├── FlexiSoftMdReader.exe
├── docs/
│   └── README.md          (references images)
│   └── images/            (image directory)
│       └── logo.png
└── examples/
    └── markdown/
        └── example_images.md    (this file)
        └── assets/
            ├── sample_image.png
            └── warning_icon.png
```

When opening `docs/README.md`, relative image reference `![](images/logo.png)` looks in `docs/images/`.

When opening `examples/markdown/example_images.md`, relative image reference `![](assets/sample_image.png)` looks in `examples/markdown/assets/`.

## What Happens if Images Don't Load

If an image file is not found or cannot be loaded, FlexiSoftMdReader displays the alt text in place of the image.

Reasons images might not load:
- File not found at the specified path
- Path is incorrect or uses wrong separators
- File is not a supported format
- File is corrupted

## Tips for Working with Images

1. **Use consistent paths** – Organize images in clear subdirectories
2. **Use relative paths** – Avoid absolute paths like `C:\...`
3. **Test locally** – Verify images display before distribution
4. **Use supported formats** – PNG is recommended for documentation
5. **Provide alt text** – Always include descriptive alt text

## Images and Localization

Image paths work the same regardless of the language of the Markdown content. You can use the same images in `README.md`, `README_cz.md`, `MANUAL_uk.md`, etc.
