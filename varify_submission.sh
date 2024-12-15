#!/bin/bash

# Prompt user for a number
read -p "Enter number, for which HW<number>/WetTest you would like Test: " NUM

# Validate input
if ! [[ "$NUM" =~ ^[0-9]+$ ]] || [ "$NUM" -lt 1 ]; then
    echo "Invalid input. Please enter a number greater than or equal to 1."
    exit 1
fi

HW_PATH="HW$NUM/WetTest"
TEMP_DIR="$HW_PATH/temp"

# Delete the temp directory if it exists
if [ -d "$TEMP_DIR" ]; then
    echo "Deleting existing temp directory..."
    rm -rf "$TEMP_DIR"
else
    echo "No temp directory found, proceeding..."
fi

# Unzip submission.zip into the temp directory
echo "Unzipping submission.zip into $TEMP_DIR..."
mkdir -p "$TEMP_DIR"  # Create temp directory if it doesn't exist
unzip -q submission.zip -d "$TEMP_DIR"

# Run the verification script
echo "Running verification script..."
if [ -f "$HW_PATH/verify_${NUM}_submission.sh" ]; then
    ./"$HW_PATH/verify_${NUM}_submission.sh"
else
    echo "Verification script not found: $HW_PATH/verify_${NUM}_submission.sh"
    exit 1
fi
