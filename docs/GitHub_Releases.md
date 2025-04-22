# GitHub Release Process for 3D Model Painter

This document explains the GitHub release process for the 3D Model Painter project.

## Release Workflow

### 1. Prepare for Release

1. Ensure all features for the release are complete and merged into the main branch
2. Run all tests to ensure everything is working correctly
3. Update documentation to reflect new features or changes
4. Update version number in `.version_info`

### 2. Update Release Notes

1. Create detailed release notes using the template in `RELEASE_NOTES_TEMPLATE.md`
2. Include:
   - New features
   - Bug fixes
   - Breaking changes
   - Known issues
   - Upgrade notes
   - Contributors

### 3. Create Release Packages

For cross-platform release:

```bash
# Check current version
cat .version_info

# Run release checker
./check_releases.sh

# Create release packages
./create_github_release.sh
```

For Windows-specific package:

```bash
# Create Windows installer
./windows_build.bat
./create_installer.bat
```

### 4. Publishing on GitHub

1. Go to your GitHub repository
2. Navigate to "Releases" section
3. Click "Draft a new release"
4. Set the tag version (e.g., v0.2.0)
5. Set the release title (e.g., "3D Model Painter v0.2.0")
6. Paste the release notes into the description
7. Upload the release packages:
   - `3D-Model-Painter-v0.2.0.tar.gz` (Linux/macOS)
   - `3D-Model-Painter-v0.2.0-Github-Release.tar.gz` (Source code)
   - `3DModelPainter_Windows.zip` (Windows package)
   - `3DModelPainter-Setup-v0.2.0.exe` (Windows installer)
8. Set as pre-release if needed
9. Publish the release

### 5. Post-Release

1. Update website with new release information
2. Announce release on relevant channels
3. Monitor for any critical issues
4. Plan next development cycle

## Versioning

We follow semantic versioning (MAJOR.MINOR.PATCH):

- MAJOR: Incompatible API changes
- MINOR: Add functionality in a backward-compatible manner
- PATCH: Backward-compatible bug fixes

## Automatic Updates

The 3D Model Painter includes automatic update detection:

1. The application checks for new releases on GitHub
2. Users are notified when a new version is available
3. The `update_from_github.sh` script can automatically update the application

## Release Schedule

- Regular releases: Every 2-3 months
- Patch releases: As needed for critical bugs
- Major releases: Announced with at least 1 month notice