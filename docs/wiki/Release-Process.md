# Release Process

This page documents the steps for creating and releasing FlexiSoftMdReader.

## Release Checklist

### 1. Prepare Code

- [ ] Verify all source code changes are committed
- [ ] Ensure no uncommitted changes remain
- [ ] Run lint/code review if applicable
- [ ] Update version in documentation (if needed)

### 2. Build Release

- [ ] Open `FlexiSoftMdReader.sln` in Visual Studio
- [ ] Select **Release | Win32** configuration
- [ ] Press **Ctrl+B** to build
- [ ] Verify build completes without errors

### 3. Verify XP Compatibility

Use `dumpbin` to verify Windows XP subsystem:

```batch
dumpbin /headers FlexiSoftMdReader\Release\FlexiSoftMdReader.exe | findstr /i "subsystem machine"
```

**Expected output**:
```
  14C machine (x86)
  5.01 subsystem version
  2 subsystem (Windows GUI)
```

If the subsystem is not 5.01, the build failed the XP compatibility test. Do not release.

### 4. Test on Windows XP (Recommended)

- [ ] Copy executable to Windows XP machine or VM
- [ ] Test opening a Markdown file
- [ ] Test font argument with system font
- [ ] Test font argument with .ttf file (if available)
- [ ] Verify images display if documentation includes them

### 5. Copy Executable

Copy the verified executable to release folders:

```batch
copy FlexiSoftMdReader\Release\FlexiSoftMdReader.exe dist\
copy FlexiSoftMdReader\Release\FlexiSoftMdReader.exe release\v0.1.0\
```

### 6. Create Distribution Package

Create a ZIP file containing the executable and documentation:

```batch
cd release\v0.1.0\
```

Include in ZIP:
- `FlexiSoftMdReader.exe`
- `README.md` (from repository root)
- `examples/README.md`
- `examples/markdown/example_basic.md`
- `examples/markdown/example_tables.md`
- `examples/markdown/example_images.md`
- `examples/markdown/assets/sample_image.png`
- `examples/markdown/assets/warning_icon.png`

Name the file: `FlexiSoftMdReader_v0.1.0_win32_xp.zip`

Example using PowerShell:

```powershell
$zip = "release\v0.1.0\FlexiSoftMdReader_v0.1.0_win32_xp.zip"
$files = @(
    "FlexiSoftMdReader\Release\FlexiSoftMdReader.exe",
    "README.md",
    "examples/README.md",
    "examples/markdown/example_basic.md",
    "examples/markdown/example_tables.md",
    "examples/markdown/example_images.md",
    "examples/markdown/assets/sample_image.png",
    "examples/markdown/assets/warning_icon.png"
)
Compress-Archive -Path $files -DestinationPath $zip
```

### 7. Calculate SHA256 Checksums

Create `release/v0.1.0/SHA256SUMS.txt`:

```batch
@echo off
certutil -hashfile FlexiSoftMdReader.exe SHA256
certutil -hashfile FlexiSoftMdReader_v0.1.0_win32_xp.zip SHA256
```

Or using PowerShell:

```powershell
$sha_exe = (Get-FileHash -Path "release\v0.1.0\FlexiSoftMdReader.exe" -Algorithm SHA256).Hash
$sha_zip = (Get-FileHash -Path "release\v0.1.0\FlexiSoftMdReader_v0.1.0_win32_xp.zip" -Algorithm SHA256).Hash
"$sha_exe  FlexiSoftMdReader.exe" | Out-File "release\v0.1.0\SHA256SUMS.txt"
"$sha_zip  FlexiSoftMdReader_v0.1.0_win32_xp.zip" | Out-File "release\v0.1.0\SHA256SUMS.txt" -Append
```

### 8. Create Release Notes

Create `release/v0.1.0/RELEASE_NOTES.md`:

```markdown
# FlexiSoftMdReader v0.1.0 Release Notes

## Build Information

- **Version**: v0.1.0
- **Platform**: Win32 (x86)
- **Windows XP Compatible**: Yes (subsystem 5.01)
- **Built with**: Visual Studio 2015+, XP Toolset v141_xp
- **Runtime**: Static (/MT) – no external DLLs required

## What's Included

- `FlexiSoftMdReader.exe` – Main application
- `FlexiSoftMdReader_v0.1.0_win32_xp.zip` – Full distribution package
- `SHA256SUMS.txt` – Checksums for verification

## Verification

Verify the executable subsystem version:

```
dumpbin /headers FlexiSoftMdReader.exe | findstr /i "subsystem machine"
```

Expected:
```
  14C machine (x86)
  5.01 subsystem version
  2 subsystem (Windows GUI)
```

## Installation

1. Extract the ZIP file
2. Copy `FlexiSoftMdReader.exe` to your FlexiSoft Runtime folder
3. Place documentation in `docs/` subfolder
4. Test: `FlexiSoftMdReader.exe "docs\README.md"`

## Supported Systems

- Windows XP SP3 and later
- Windows Vista through Windows 11
- Any 32-bit Windows system

## Known Limitations

- Simple Markdown viewer, not full GitHub renderer
- Read-only document display
- Basic table rendering
- No script execution

## For More Information

- **GitHub**: https://github.com/KVLab/FlexiSoftMdReader
- **Usage**: See examples/ and docs/wiki/ in distribution
```

### 9. Commit and Push

If you've made changes to the repository:

```batch
git add -A
git commit -m "Release v0.1.0: Initial release with Windows XP support"
git push origin master
```

### 10. Create GitHub Release (Manual)

This step is currently **manual**:

1. Go to https://github.com/KVLab/FlexiSoftMdReader
2. Click **Releases** → **Create a new release**
3. Tag: `v0.1.0`
4. Title: `FlexiSoftMdReader v0.1.0 - Windows XP Compatible Release`
5. Description: Copy from `RELEASE_NOTES.md`
6. **Attach files**:
   - Upload `FlexiSoftMdReader_v0.1.0_win32_xp.zip`
   - Upload `SHA256SUMS.txt`
7. Click **Publish release**

### 11. Verify Release

- [ ] GitHub release is published
- [ ] Attached files are correct
- [ ] SHA256SUMS match
- [ ] Download and test the ZIP file

## Automated Release (Future)

To automate this process with GitHub Actions, create:

1. `.github/workflows/release.yml` – Automated build on tag
2. Archive creation script
3. Checksum generation
4. GitHub Release creation

Currently, releases are created manually.

## Version Numbering

Use semantic versioning:

- `v0.1.0` – Initial release
- `v0.2.0` – New features
- `v0.1.1` – Bug fixes
- `v1.0.0` – Stable release

## Long-Term Maintenance

- [ ] Track issues on GitHub
- [ ] Document bugs as GitHub issues
- [ ] Plan future releases
- [ ] Update documentation
- [ ] Keep XP compatibility in all updates
- [ ] Test on Windows XP with each release
