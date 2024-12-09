#!/bin/bash
# generate_file_list.sh
# Generates a list of files to zip and saves them to a temporary file.
cd HW1/Wet/code

OUTPUT_FILE="/../../../files_to_zip.txt" # This is the output file with the list of files

# Pre-defined list of files (modify this list as needed)
FILES=(
    "Commands.h"
    "Commands.cpp"
    "signals.h"
    "signals.cpp"
    "smash.cpp"
    "Makefile"
    # "submitters.txt" (part of GLOBAL_FILES)
)

# Write the file paths to the output file
for file in "${FILES[@]}"; do
    echo "$file" >> "$OUTPUT_FILE"
done

echo "File list generated in $OUTPUT_FILE"
