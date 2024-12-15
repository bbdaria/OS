#!/bin/bash
# generate_file_list.sh
# Dynamically generates a list of .cpp and .h files to zip, along with additional files.

RELATIVE_PATH="HW1/Wet/code"
OUTPUT_FILE="files_to_zip.txt" # This is the output file with the list of files

cd $RELATIVE_PATH || { echo "Error: Directory $RELATIVE_PATH not found."; exit 1; }

# Dynamically find all .cpp and .h files under the code directory
find . -type f \( -name "*.cpp" -o -name "*.h" \) | sed "s|^./|$RELATIVE_PATH/|" >> "../../../$OUTPUT_FILE"

# Include additional files such as Makefile
ADDITIONAL_FILES=(
    "Makefile"
    # "../submitters.txt" (will be added in GLOBAL_FILES)
)

# Append additional files to the output file
for file in "${ADDITIONAL_FILES[@]}"; do
    echo "$RELATIVE_PATH/$file" >> "../../../$OUTPUT_FILE"
done

cd -

echo "File list generated in $OUTPUT_FILE"
