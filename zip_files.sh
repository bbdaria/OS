#!/bin/bash
# zip_files.sh
# Zips the files listed in a specified list file into a zip archive.

LIST_FILE=$1       # The first argument is the list file generated by generate_file_list.sh
ZIP_NAME="submission.zip" # Name of the zip file to create

if [ -z "$LIST_FILE" ]; then
    echo "Usage: $0 <list_file>"
    exit 1
fi

if [ ! -f "$LIST_FILE" ]; then
    echo "List file $LIST_FILE does not exist."
    exit 1
fi

# Read the list file and zip the files
# zip -v "$ZIP_NAME" < "$LIST_FILE"
zip -@ "$ZIP_NAME" < "$LIST_FILE"

echo "Files zipped into $ZIP_NAME"