# Troubleshooting

Common issues and solutions.

## Application Won't Start

### Problem: "File not found" error

**Cause**: The Markdown file path is incorrect or the file doesn't exist.

**Solution**:
- Verify the file path: `dir "path\to\file.md"`
- Use absolute paths for testing
- Check for typos in the filename

### Problem: No window appears

**Cause**: The executable may have crashed silently or the window opened off-screen.

**Solution**:
- Try a different Markdown file
- Test with a simple text file
- Verify the executable with: `FlexiSoftMdReader.exe --help` (if supported)
- Check Windows Event Viewer for crash information

## Images Not Displaying

### Problem: Image shows broken/missing

**Cause**: Image file not found or incorrect path.

**Solution**:
1. Check the image path in the Markdown file:
   ```markdown
   ![alt](assets/image.png)
   ```
2. Verify the image file exists:
   ```batch
   dir "assets\image.png"
   ```
3. Ensure paths use backslashes (`\`) not forward slashes (`/`)
4. Image paths are relative to the Markdown file directory:
   - If opening `docs/README.md`
   - Reference `![](images/logo.png)`
   - Looks for `docs/images/logo.png`

### Problem: Unsupported image format

**Cause**: Image format not supported.

**Solution**:
- Supported formats: **PNG, JPG/JPEG, GIF, BMP**
- Convert images to PNG (most reliable)
- Use online converters or ImageMagick

## Font Problems

### Problem: Font not loading

**Cause**: Font file not found or system font not installed.

**Solution**:
1. Verify the font file exists:
   ```batch
   dir "fonts\CustomFont.ttf"
   ```
2. Try without the font argument (uses default):
   ```batch
   FlexiSoftMdReader.exe "docs\README.md"
   ```
3. Try a common system font:
   ```batch
   FlexiSoftMdReader.exe "docs\README.md" "Tahoma"
   ```

### Problem: Wrong font displayed

**Cause**: Font face name misspelled or not installed.

**Solution**:
- Check system fonts: Open Control Panel → Fonts
- Verify font name spelling
- Use standard fonts: Tahoma, Arial, Courier New, Times New Roman
- The viewer falls back to default if font not found (this is OK)

### Problem: International characters appear as boxes

**Cause**: Font doesn't support the character set.

**Solution**:
- Use Unicode fonts: Tahoma, Arial, Segoe UI
- For specific languages, use language-specific fonts
- NotoSans supports most Unicode scripts
- Verify Markdown file uses UTF-8 encoding

## Windows XP Compatibility Issues

### Problem: "This program cannot run on this computer" (Windows XP)

**Cause**: Executable was not built with Windows XP subsystem.

**Solution**:
1. Verify the build used `v141_xp` toolset
2. Check subsystem version:
   ```batch
   dumpbin /headers FlexiSoftMdReader.exe | findstr /i "subsystem"
   ```
3. Expected: `5.01 subsystem version`
4. Rebuild with correct toolset if needed

### Problem: Missing dependencies on Windows XP

**Cause**: Windows XP doesn't have modern runtime libraries.

**Solution**:
- Ensure build used **static runtime (/MT)**
- No external DLLs should be required
- Verify with: `dumpbin /dependents FlexiSoftMdReader.exe`
- Should only show system DLLs (kernel32.dll, user32.dll, gdi32.dll)

## Markdown Rendering Issues

### Problem: Markdown not parsed correctly

**Cause**: Markdown syntax not recognized.

**Solution**:
- Check supported features: See [Markdown-Support.md](Markdown-Support.md)
- Unsupported features won't render
- Use simple Markdown structures
- Avoid advanced GitHub-specific features

### Problem: Tables appear as plain text

**Cause**: Tables are intentionally rendered as text, not HTML.

**Solution**:
- This is expected behavior
- Tables display as readable preformatted text
- Content is still legible
- For full table support, use other tools

### Problem: Code block not highlighted

**Cause**: Syntax highlighting may not be supported.

**Solution**:
- Content is still displayed (just not colored)
- Focus on readability
- Use clear formatting in code
- Add comments for clarity

## File Encoding Issues

### Problem: Characters appear garbled

**Cause**: File encoding is not UTF-8.

**Solution**:
1. Save file as UTF-8 (with or without BOM)
2. In text editor: Save As → Encoding UTF-8
3. Verify: Open file in Notepad → Encoding
4. Should say "UTF-8"

## Performance Issues

### Problem: Viewer is slow to open

**Cause**: Large Markdown file or slow system.

**Solution**:
- This is expected for very large files
- Wait for the window to appear
- On Windows XP, add extra time
- Consider splitting very large documents

### Problem: Viewer is unresponsive

**Cause**: Application is processing large file.

**Solution**:
- Wait for processing to complete
- Try with smaller test file
- Restart and try again
- Report issue if persists

## Integration with FlexiSoft Runtime

### Problem: Runtime can't find FlexiSoftMdReader.exe

**Cause**: Executable not in expected location.

**Solution**:
1. Verify executable is in runtime folder
2. Check path specified in runtime code
3. Use absolute path for testing
4. Verify file name spelling (case-insensitive on Windows)

### Problem: Runtime passes wrong font argument

**Cause**: Font specification issue.

**Solution**:
- Verify runtime passes existing font name or valid file path
- Test manually: `FlexiSoftMdReader.exe "docs\README.md" "Tahoma"`
- Add validation in runtime code

## Getting Help

If issue persists:

1. Check [docs/wiki/](../wiki/) for more detailed documentation
2. Search GitHub issues: https://github.com/KVLab/FlexiSoftMdReader/issues
3. Test with `dumpbin` to verify executable integrity
4. Verify Windows XP subsystem: `dumpbin /headers FlexiSoftMdReader.exe`
5. Report detailed steps to reproduce
