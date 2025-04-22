#!/bin/bash
# Script to check for updates on GitHub and automatically update files

# Configuration
REPO_OWNER="3D-Model-Painter"  # Replace with your actual GitHub username/organization
REPO_NAME="3D-Model-Painter"   # Replace with your actual repository name
VERSION_FILE=".version_info"
RELEASE_ASSETS_DIR="downloaded_releases"

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  3D Model Painter Auto-Update Tool    ${NC}"
echo -e "${BLUE}========================================${NC}"

# Check if jq is installed
if ! command -v jq &> /dev/null; then
    echo -e "${RED}Error: This script requires jq for JSON parsing.${NC}"
    echo -e "${YELLOW}Please install jq:${NC}"
    echo "  - On Ubuntu/Debian: sudo apt-get install jq"
    echo "  - On CentOS/RHEL: sudo yum install jq"
    echo "  - On macOS: brew install jq"
    echo "  - On Windows (with Chocolatey): choco install jq"
    exit 1
fi

# Create version file if it doesn't exist
if [ ! -f "$VERSION_FILE" ]; then
    echo "0.0.0" > "$VERSION_FILE"
    echo -e "${YELLOW}Created version tracking file.${NC}"
fi

# Create assets directory if it doesn't exist
mkdir -p "$RELEASE_ASSETS_DIR"

# Read current known version
CURRENT_VERSION=$(cat "$VERSION_FILE")
echo -e "Current version: ${YELLOW}$CURRENT_VERSION${NC}"

# Fetch the latest release info from GitHub API
echo -e "${BLUE}Fetching latest release information...${NC}"
RELEASE_INFO=$(curl -s "https://api.github.com/repos/$REPO_OWNER/$REPO_NAME/releases/latest")

# Check if API request was successful
if [[ $RELEASE_INFO == *"API rate limit exceeded"* ]]; then
    echo -e "${YELLOW}GitHub API rate limit exceeded. Try again later.${NC}"
    exit 1
fi

if [[ $RELEASE_INFO == *"Not Found"* ]]; then
    echo -e "${YELLOW}Repository or releases not found. Check repository name and owner.${NC}"
    echo -e "${YELLOW}You may need to create your first release on GitHub.${NC}"
    exit 1
fi

# Extract version and release info
LATEST_VERSION=$(echo "$RELEASE_INFO" | jq -r '.tag_name')
RELEASE_URL=$(echo "$RELEASE_INFO" | jq -r '.html_url')
RELEASE_DATE=$(echo "$RELEASE_INFO" | jq -r '.published_at' | cut -d'T' -f1)
RELEASE_BODY=$(echo "$RELEASE_INFO" | jq -r '.body')

# If no releases found
if [ -z "$LATEST_VERSION" ] || [ "$LATEST_VERSION" == "null" ]; then
    echo -e "${YELLOW}No releases found. Make sure you have published releases on GitHub.${NC}"
    exit 1
fi

echo -e "Latest version: ${GREEN}$LATEST_VERSION${NC} (Released: $RELEASE_DATE)"

# Compare versions
if [ "$CURRENT_VERSION" != "$LATEST_VERSION" ]; then
    echo -e "${GREEN}New version detected! Processing update...${NC}"
    
    # Update version info file
    echo "$LATEST_VERSION" > "$VERSION_FILE"
    
    # Update the README.md with new release info
    if [ -f "README.md" ]; then
        # Update README with release info using the check_releases.sh script
        if [ -f "./check_releases.sh" ]; then
            echo -e "${BLUE}Updating README with new release information...${NC}"
            chmod +x ./check_releases.sh
            ./check_releases.sh
        fi
    fi
    
    # Download release assets
    echo -e "${BLUE}Checking for downloadable assets...${NC}"
    ASSETS=$(echo "$RELEASE_INFO" | jq -r '.assets')
    ASSET_COUNT=$(echo "$ASSETS" | jq -r '. | length')
    
    if [ "$ASSET_COUNT" -gt 0 ]; then
        echo -e "${GREEN}Found $ASSET_COUNT assets in release.${NC}"
        
        # Create a directory for this specific version
        VERSION_DIR="$RELEASE_ASSETS_DIR/$LATEST_VERSION"
        mkdir -p "$VERSION_DIR"
        
        # Download each asset
        for (( i=0; i<$ASSET_COUNT; i++ )); do
            ASSET_NAME=$(echo "$ASSETS" | jq -r ".[$i].name")
            ASSET_URL=$(echo "$ASSETS" | jq -r ".[$i].browser_download_url")
            ASSET_SIZE=$(echo "$ASSETS" | jq -r ".[$i].size")
            
            # Convert size to human-readable format
            if [ $ASSET_SIZE -ge 1048576 ]; then
                SIZE_HUMAN=$(echo "scale=2; $ASSET_SIZE/1048576" | bc)
                SIZE_HUMAN="${SIZE_HUMAN} MB"
            else
                SIZE_HUMAN=$(echo "scale=2; $ASSET_SIZE/1024" | bc)
                SIZE_HUMAN="${SIZE_HUMAN} KB"
            fi
            
            echo -e "${BLUE}Downloading: ${YELLOW}$ASSET_NAME${BLUE} ($SIZE_HUMAN)${NC}"
            curl -L -o "$VERSION_DIR/$ASSET_NAME" "$ASSET_URL"
            
            if [ $? -eq 0 ]; then
                echo -e "${GREEN}Downloaded $ASSET_NAME successfully!${NC}"
                # If it's a zip file, offer to extract it
                if [[ "$ASSET_NAME" == *.zip ]]; then
                    echo -e "${YELLOW}Would you like to extract $ASSET_NAME? (y/n)${NC}"
                    read -r EXTRACT_CHOICE
                    if [[ "$EXTRACT_CHOICE" =~ ^[Yy]$ ]]; then
                        EXTRACT_DIR="$VERSION_DIR/${ASSET_NAME%.zip}"
                        mkdir -p "$EXTRACT_DIR"
                        echo -e "${BLUE}Extracting to $EXTRACT_DIR...${NC}"
                        unzip -q "$VERSION_DIR/$ASSET_NAME" -d "$EXTRACT_DIR"
                        echo -e "${GREEN}Extraction complete!${NC}"
                    fi
                fi
            else
                echo -e "${RED}Failed to download $ASSET_NAME.${NC}"
            fi
        done
        
        echo -e "${GREEN}All assets downloaded to: $VERSION_DIR${NC}"
    else
        echo -e "${YELLOW}No downloadable assets found for this release.${NC}"
    fi
    
    # Check if there's a release notes file for this version and create it if not
    RELEASE_NOTES_FILE="$LATEST_VERSION-RELEASE_NOTES.md"
    if [ ! -f "$RELEASE_NOTES_FILE" ]; then
        echo -e "${BLUE}Creating release notes file...${NC}"
        cat > "$RELEASE_NOTES_FILE" << EOF
# 3D Model Painter $LATEST_VERSION - Release Notes

Released on: $RELEASE_DATE
[View on GitHub]($RELEASE_URL)

## What's New:
$RELEASE_BODY

## Installation
1. Download the latest version from [GitHub Releases]($RELEASE_URL)
2. Extract the ZIP file to a folder of your choice
3. Follow the instructions in README.txt

## Windows Installation
For Windows users, an installer is available that will:
- Install the application to Program Files
- Create shortcuts in Start Menu and Desktop
- Add the application to Windows taskbar
- Register file associations

## Questions or Issues?
Please report any issues on GitHub: https://github.com/$REPO_OWNER/$REPO_NAME/issues
EOF
        echo -e "${GREEN}Created release notes: $RELEASE_NOTES_FILE${NC}"
    fi
    
    echo -e "${GREEN}Update to version $LATEST_VERSION complete!${NC}"
    echo -e "${BLUE}You can now review the release notes and downloaded assets.${NC}"
else
    echo -e "${BLUE}Your application is up to date (Version: $CURRENT_VERSION).${NC}"
fi

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Update check completed successfully   ${NC}"
echo -e "${BLUE}========================================${NC}"

exit 0