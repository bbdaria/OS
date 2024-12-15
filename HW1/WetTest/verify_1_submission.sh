#!/bin/bash

# Define paths
HW_PATH="HW1/WetTest"
TEST_DIR="HW1/WetTest/tests"

# Change to the temp directory
cd "$HW_PATH/temp" || { echo "Failed to change directory to $HW_PATH/temp"; exit 1; }

# Build the executable
make smash

# Check if 'smash' executable exists
if [ -f "smash" ]; then
    echo -e "\033[0;32mCompiled Successfully!\033[0m"

    # Go back to HW_PATH
    cd ..

    # Copy the 'smash' executable to the tests directory
    cp temp/smash "./"
    echo "smash executable successfully copied to $HW_PATH"

    # Run the runner.sh script
    ./tests/runner/runner.sh

else
    echo "Error: 'smash' executable not found."
    exit 1
fi
