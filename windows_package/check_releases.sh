#!/bin/bash
# Script to check for new GitHub releases and update README.md

# Configuration
REPO_OWNER="3D-Model-Painter"  # Replace with your actual GitHub username/organization
REPO_NAME="3D-Model-Painter"   # Replace with your actual repository name
README_FILE="README.md"
VERSION_FILE=".version_info"

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}Checking for new GitHub releases...${NC}"

# Create version file if it doesn't exist
if [ ! -f "$VERSION_FILE" ]; then
    echo "0.0.0" > "$VERSION_FILE"
    echo -e "${YELLOW}Created version tracking file.${NC}"
fi

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
LATEST_VERSION=$(echo "$RELEASE_INFO" | grep -o '"tag_name": *"[^"]*"' | head -n 1 | sed 's/"tag_name": *"//;s/"//')
RELEASE_URL=$(echo "$RELEASE_INFO" | grep -o '"html_url": *"[^"]*"' | head -n 1 | sed 's/"html_url": *"//;s/"//')
RELEASE_DATE=$(echo "$RELEASE_INFO" | grep -o '"published_at": *"[^"]*"' | head -n 1 | sed 's/"published_at": *"//;s/".*//' | cut -d'T' -f1)
RELEASE_BODY=$(echo "$RELEASE_INFO" | grep -o '"body": *"[^"]*"' | sed 's/"body": *"//;s/\\r\\n/\n/g;s/\\n/\n/g;s/"//')

# If no releases found
if [ -z "$LATEST_VERSION" ]; then
    echo -e "${YELLOW}No releases found. Make sure you have published releases on GitHub.${NC}"
    exit 1
fi

echo -e "Latest version: ${GREEN}$LATEST_VERSION${NC} (Released: $RELEASE_DATE)"

# Compare versions (this is a simple comparison and might not work for all versioning schemes)
if [ "$CURRENT_VERSION" != "$LATEST_VERSION" ]; then
    echo -e "${GREEN}New version detected! Updating README...${NC}"
    
    # Update version info file
    echo "$LATEST_VERSION" > "$VERSION_FILE"
    
    # Update the README.md with new release info
    # This pattern searches for a specific marker section and replaces content between markers
    if grep -q "<!-- RELEASE_INFO_START -->" "$README_FILE"; then
        # Create the new release section
        RELEASE_SECTION="<!-- RELEASE_INFO_START -->
## Latest Release: $LATEST_VERSION

Released on: $RELEASE_DATE
[View on GitHub]($RELEASE_URL)

### What's New:
$RELEASE_BODY
<!-- RELEASE_INFO_END -->"
        
        # Replace the old release section with the new one
        sed -i".bak" "/<!-- RELEASE_INFO_START -->/,/<!-- RELEASE_INFO_END -->/c\\$RELEASE_SECTION" "$README_FILE"
        rm "${README_FILE}.bak" # Remove backup file
        
        echo -e "${GREEN}README.md successfully updated with the latest release information!${NC}"
    else
        # If markers don't exist, add the release section at the end
        echo -e "${YELLOW}Release section markers not found in README. Adding section...${NC}"
        cat >> "$README_FILE" << EOF

<!-- RELEASE_INFO_START -->
## Latest Release: $LATEST_VERSION

Released on: $RELEASE_DATE
[View on GitHub]($RELEASE_URL)

### What's New:
$RELEASE_BODY
<!-- RELEASE_INFO_END -->
EOF
        echo -e "${GREEN}Release information added to README.md!${NC}"
    fi
    
    # Add download badge if it doesn't exist
    if ! grep -q "img.shields.io.*release" "$README_FILE"; then
        echo -e "${YELLOW}Adding release badge to README...${NC}"
        # Find the first header line and add badge after it
        sed -i".bak" "1s/^/# 3D Model Painter\n\n[![Latest Release](https:\/\/img.shields.io\/github\/v\/release\/$REPO_OWNER\/$REPO_NAME?style=flat-square&logo=github)](https:\/\/github.com\/$REPO_OWNER\/$REPO_NAME\/releases\/latest)\n\n/" "$README_FILE"
        rm "${README_FILE}.bak" # Remove backup file
        echo -e "${GREEN}Release badge added!${NC}"
    else
        # Update badge version
        sed -i".bak" "s/img.shields.io\/github\/v\/release\/[^?]*/$REPO_OWNER\/$REPO_NAME/" "$README_FILE"
        rm "${README_FILE}.bak" # Remove backup file
    fi
    
    echo -e "${GREEN}README.md update complete!${NC}"
    echo -e "${BLUE}You can now commit and push the changes to GitHub.${NC}"
else
    echo -e "${BLUE}No new version detected. README is up to date.${NC}"
fi

exit 0