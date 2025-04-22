#!/bin/bash
# Script to create a GitHub release with the current version

# Configuration
REPO_OWNER="3D-Model-Painter"  # Replace with your actual GitHub username/organization
REPO_NAME="3D-Model-Painter"   # Replace with your actual repository name
VERSION_FILE=".version_info"
RELEASE_NOTES_TEMPLATE="RELEASE_NOTES_TEMPLATE.md"

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  3D Model Painter GitHub Release Tool  ${NC}"
echo -e "${BLUE}========================================${NC}"

# Check if GitHub CLI is installed
if ! command -v gh &> /dev/null; then
    echo -e "${RED}Error: This script requires GitHub CLI (gh) to create releases.${NC}"
    echo -e "${YELLOW}Please install GitHub CLI:${NC}"
    echo "  - Visit: https://cli.github.com/"
    echo "  - Follow the installation instructions for your platform"
    exit 1
fi

# Check if logged in to GitHub
echo -e "${BLUE}Checking GitHub authentication...${NC}"
gh auth status &> /dev/null
if [ $? -ne 0 ]; then
    echo -e "${RED}You are not logged in to GitHub CLI.${NC}"
    echo -e "${YELLOW}Please run 'gh auth login' to authenticate.${NC}"
    exit 1
fi
echo -e "${GREEN}Authenticated with GitHub CLI${NC}"

# Determine version
if [ -f "$VERSION_FILE" ]; then
    CURRENT_VERSION=$(cat "$VERSION_FILE")
    echo -e "Current version from file: ${YELLOW}$CURRENT_VERSION${NC}"
else
    echo -e "${YELLOW}No version file found. Creating one...${NC}"
    
    # Ask for current version
    echo -e "${YELLOW}Please enter the version number (e.g., v1.0.0):${NC}"
    read -r CURRENT_VERSION
    
    # Save to version file
    echo "$CURRENT_VERSION" > "$VERSION_FILE"
    echo -e "${GREEN}Version saved to $VERSION_FILE${NC}"
fi

# Ask if user wants to update version
echo -e "${YELLOW}Current version is $CURRENT_VERSION. Would you like to change it? (y/n)${NC}"
read -r CHANGE_VERSION
if [[ "$CHANGE_VERSION" =~ ^[Yy]$ ]]; then
    echo -e "${YELLOW}Please enter the new version number:${NC}"
    read -r NEW_VERSION
    
    # Update version file
    echo "$NEW_VERSION" > "$VERSION_FILE"
    CURRENT_VERSION="$NEW_VERSION"
    echo -e "${GREEN}Version updated to $CURRENT_VERSION${NC}"
    
    # Update README if check_releases.sh exists
    if [ -f "./check_releases.sh" ]; then
        echo -e "${BLUE}Updating README with new version...${NC}"
        chmod +x ./check_releases.sh
        ./check_releases.sh
    fi
fi

# Create temporary release notes file
TEMP_RELEASE_NOTES=$(mktemp)

# Check if template exists
if [ -f "$RELEASE_NOTES_TEMPLATE" ]; then
    # Use template
    cat "$RELEASE_NOTES_TEMPLATE" > "$TEMP_RELEASE_NOTES"
    
    # Replace placeholders
    sed -i "s/{{VERSION}}/$CURRENT_VERSION/g" "$TEMP_RELEASE_NOTES"
    sed -i "s/{{DATE}}/$(date +%Y-%m-%d)/g" "$TEMP_RELEASE_NOTES"
else
    # Create simple release notes
    cat > "$TEMP_RELEASE_NOTES" << EOF
# 3D Model Painter $CURRENT_VERSION

## What's New
- [Add your changes here]
- [Add more changes here]

## Bug Fixes
- [Add bug fixes here]

## Known Issues
- [List any known issues here]

## Installation
Download the attached assets and follow the instructions in README.txt.
EOF
fi

# Open editor to edit release notes
if command -v nano &> /dev/null; then
    echo -e "${YELLOW}Opening release notes in nano editor...${NC}"
    nano "$TEMP_RELEASE_NOTES"
elif command -v vim &> /dev/null; then
    echo -e "${YELLOW}Opening release notes in vim editor...${NC}"
    vim "$TEMP_RELEASE_NOTES"
else
    echo -e "${YELLOW}No editor found. Please edit the file manually:${NC}"
    echo -e "${BLUE}$TEMP_RELEASE_NOTES${NC}"
    echo -e "${YELLOW}Press Enter when done editing${NC}"
    read -r
fi

# Create release notes markdown file
RELEASE_NOTES_FILE="$CURRENT_VERSION-RELEASE_NOTES.md"
cp "$TEMP_RELEASE_NOTES" "$RELEASE_NOTES_FILE"
echo -e "${GREEN}Created release notes: $RELEASE_NOTES_FILE${NC}"

# Prepare assets
echo -e "${BLUE}Preparing release assets...${NC}"
ASSETS=()

# Run Windows package creation if script exists
if [ -f "./create_windows_package.sh" ]; then
    echo -e "${YELLOW}Would you like to create a Windows package? (y/n)${NC}"
    read -r CREATE_WINDOWS
    if [[ "$CREATE_WINDOWS" =~ ^[Yy]$ ]]; then
        echo -e "${BLUE}Creating Windows package...${NC}"
        chmod +x ./create_windows_package.sh
        ./create_windows_package.sh
        
        if [ -f "3DModelPainter_Windows.zip" ]; then
            ASSETS+=("3DModelPainter_Windows.zip")
            echo -e "${GREEN}Added Windows package to release assets${NC}"
        fi
    fi
fi

# Ask if user wants to add other assets
echo -e "${YELLOW}Would you like to add additional files to the release? (y/n)${NC}"
read -r ADD_ASSETS
if [[ "$ADD_ASSETS" =~ ^[Yy]$ ]]; then
    while true; do
        echo -e "${YELLOW}Enter the path to the file (or 'done' to finish):${NC}"
        read -r ASSET_PATH
        
        if [ "$ASSET_PATH" == "done" ]; then
            break
        fi
        
        if [ -f "$ASSET_PATH" ]; then
            ASSETS+=("$ASSET_PATH")
            echo -e "${GREEN}Added $ASSET_PATH to release assets${NC}"
        else
            echo -e "${RED}File not found: $ASSET_PATH${NC}"
        fi
    done
fi

# Create GitHub release
echo -e "${BLUE}Creating GitHub release $CURRENT_VERSION...${NC}"

# First check if the release already exists
gh release view "$CURRENT_VERSION" --repo "$REPO_OWNER/$REPO_NAME" &> /dev/null
if [ $? -eq 0 ]; then
    echo -e "${YELLOW}Release $CURRENT_VERSION already exists.${NC}"
    echo -e "${YELLOW}Would you like to delete it and create a new one? (y/n)${NC}"
    read -r DELETE_RELEASE
    if [[ "$DELETE_RELEASE" =~ ^[Yy]$ ]]; then
        echo -e "${BLUE}Deleting existing release...${NC}"
        gh release delete "$CURRENT_VERSION" --repo "$REPO_OWNER/$REPO_NAME" --yes
    else
        echo -e "${YELLOW}Aborting release creation.${NC}"
        rm "$TEMP_RELEASE_NOTES"
        exit 0
    fi
fi

# Create the release
RELEASE_COMMAND="gh release create \"$CURRENT_VERSION\" --repo \"$REPO_OWNER/$REPO_NAME\" --title \"3D Model Painter $CURRENT_VERSION\" --notes-file \"$TEMP_RELEASE_NOTES\""

# Add assets
for ASSET in "${ASSETS[@]}"; do
    RELEASE_COMMAND+=" \"$ASSET\""
done

# Execute the release command
eval "$RELEASE_COMMAND"

if [ $? -eq 0 ]; then
    echo -e "${GREEN}Successfully created release $CURRENT_VERSION!${NC}"
else
    echo -e "${RED}Failed to create release. Please check the error messages above.${NC}"
fi

# Clean up
rm "$TEMP_RELEASE_NOTES"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  GitHub Release Process Completed      ${NC}"
echo -e "${BLUE}========================================${NC}"

exit 0