#!/bin/bash
# generate_file_list.sh
# Generates a list of files to zip and saves them to a temporary file.

RELATIVE_PATH="HW1/Wet/code"
OUTPUT_FILE="files_to_zip.txt" # This is the output file with the list of files

cd $RELATIVE_PATH

# Pre-defined list of files (modify this list as needed)
FILES=(
    "commands/Commands.h"
    
    "commands/built_in/built_in_command.h"
    "commands/built_in/alias/alias_command.h"
    "commands/built_in/alias/unalias_command.h"
    "commands/built_in/change_dir/change_dir_command.h"
    "commands/built_in/chprompt/chprompt_command.h"
    "commands/built_in/foreground/foreground_command.h"
    "commands/built_in/get_current_dir/get_current_dir_command.h"
    "commands/built_in/jobs/jobs_command.h"
    "commands/built_in/kill/kill_command.h"
    "commands/built_in/quit/quit_command.h"
    "commands/built_in/show_pid/show_pid_command.h"
    "commands/built_in/special/listdir/listdir_command.h"
    "commands/built_in/special/net_info/net_info_command.h"
    "commands/built_in/special/whoami/who_am_i_command.h"

    "commands/external/external_command.h"
    "commands/external/complex/complex_command.h"
    "commands/external/simple/simple_command.h"
    "commands/external/special/pipe/pipe_command.h"

    "joblist/joblist.h"
    "signals/signals.cpp"
    "signals/signals.h"
    "smash/smash.cpp"
    "smash/smash.h"
    "main.cpp"
    "util.cpp"
    "Makefile"
    # "submitters.txt" (part of GLOBAL_FILES)
)

# Write the file paths to the output file
for file in "${FILES[@]}"; do
    echo "$RELATIVE_PATH/$file" >> "../../../$OUTPUT_FILE"
done

cd ../../..
echo "File list generated in $OUTPUT_FILE"
