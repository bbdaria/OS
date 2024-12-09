#!/bin/bash
# generate_zip.sh
# Generates submission zip of all relevant files of HW<X> for input X

# prompt user Hello:
echo "Is Your HW done yet?! ready to Test?\n"
echo "Generate a zip file:\n"

# Prompt user for a number
read -p "Enter number, for which HW<number>/Wet you would like to zip: " NUM

# Validate input
if ! [[ "$NUM" =~ ^[0-9]+$ ]] || [ "$NUM" -lt 1 ]; then
    echo "Invalid input. Please enter a number greater than or equal to 1."
    exit 1
fi

PATH="HW$NUM/Wet"

chmod +x zip_files.sh
chmod +x $PATH/generate_file_list.sh

# This is the output file with the list of files to zip:
OUTPUT_FILE="files_to_zip.txt"

# Clear the output file if it exists
> "$OUTPUT_FILE"
# Pre-defined list of global-files (files shared by all HW<X>)
GLOBAL_FILES=(
    "submitters.txt"
)

# Write the file paths to the output file
for file in "${GLOBAL_FILES[@]}"; do
    echo "$file" >> "$OUTPUT_FILE"
done

./$PATH/relevant_files.sh

./zip_files.sh files_to_zip.txt
