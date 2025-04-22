# Contributing to 3D Model Painter

Thank you for your interest in contributing to the 3D Model Painter project! This document provides guidelines and instructions for contributing.

## Code of Conduct

By participating in this project, you agree to maintain a respectful and inclusive environment for everyone.

## How to Contribute

### Reporting Bugs

1. Check if the bug has already been reported in the Issues section
2. If not, create a new issue with a clear title and description
3. Include steps to reproduce, expected behavior, and actual behavior
4. Add screenshots if relevant
5. Mention your operating system and version of the application

### Suggesting Features

1. Check if the feature has already been suggested in the Issues section
2. If not, create a new issue with a clear title and description
3. Explain why this feature would be useful to most users
4. Include any relevant examples or mockups

### Pull Requests

1. Fork the repository
2. Create a new branch with a descriptive name
3. Make your changes
4. Ensure your code follows the project style guidelines
5. Test your changes thoroughly
6. Submit a pull request with a clear description of the changes

## Development Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/3D-Model-Painter.git
   cd 3D-Model-Painter
   ```

2. Install dependencies based on your operating system:
   - Linux: `sudo apt-get install libgl1-mesa-dev libglm-dev libglfw3-dev`
   - macOS: `brew install glm glfw`
   - Windows: See WindowsBuild.md

3. Run the installation script:
   ```bash
   ./install.sh
   ```

4. Run the application:
   ```bash
   ./run_3d_model_painter.sh
   ```

## Project Structure

The 3D Model Painter project is organized into several key directories:

```
3D-Model-Painter/
├── src/                   # Core source files for full 3D version
│   ├── shaders/           # GLSL shaders for OpenGL rendering
│   ├── application.cpp    # Main application logic
│   └── ...
├── build/                 # Build directory for full 3D version
├── simple_build/          # Build directory for simple version
├── docs/                  # Documentation
│   ├── index.md           # Documentation homepage
│   ├── Color_Wheel.md     # Color wheel documentation
│   ├── Export_Formats.md  # Export formats documentation
│   └── ...
├── windows_package/       # Windows-specific packaging
├── *.cpp                  # Main application variants
├── *.sh                   # Various shell scripts
├── *.bat                  # Windows batch scripts
└── *.md                   # Documentation files
```

See the full project organization in [docs/Project_Organization.md](docs/Project_Organization.md).

## Coding Guidelines

- Use consistent indentation (4 spaces)
- Follow C++ naming conventions
- Add comments for complex logic
- Write meaningful commit messages
- Place class definitions in appropriate header files
- Implement class methods in corresponding cpp files
- Keep functions short and focused on a single task
- Use namespaces to avoid conflicts (e.g., `color_wheel::` namespace)

## Documentation Guidelines

When adding or modifying features, please update the documentation:

1. Update relevant files in the `docs/` directory
2. For new features, create a new documentation file if needed
3. Update `docs/index.md` to link to new documentation
4. Include examples of how to use the feature
5. Add screenshots or diagrams if helpful

## Testing

Before submitting a pull request, please test your changes:

1. Test on your primary platform (Windows, macOS, or Linux)
2. If possible, test on at least one other platform
3. Test all application variants affected by your changes
4. Verify that documentation is accurate and up-to-date

## Creating Releases

The release process is documented in detail in [docs/GitHub_Releases.md](docs/GitHub_Releases.md).

Key steps:

1. Update version number in `.version_info` file
2. Create release notes based on `RELEASE_NOTES_TEMPLATE.md`
3. Run `./check_releases.sh` to verify version info
4. Run `./create_github_release.sh` to create the release
5. For Windows releases, run `./create_installer.bat`

## License

By contributing to 3D Model Painter, you agree that your contributions will be licensed under the project's MIT License.