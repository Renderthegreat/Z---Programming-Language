#!/bin/bash

# Check if at least two arguments are passed: output file and one input file
if [ "$#" -lt 2 ]; then
  echo "Usage: $0 output_file.h input_file1 [input_file2 ...]"
  exit 1
fi

# First argument is the output header file
output_file="$1"
shift  # Shift the arguments so that "$@" only contains the input files

current_unix_time=$(date +%s)

# Empty or create the output file
> "$output_file"

# Iterate over all input files
for input_file in "$@"; do
  # Convert the file name to uppercase and replace non-alphanumeric characters with underscores
  define_name=$(echo "$(basename "$input_file" | tr '[:lower:]' '[:upper:]' | tr -c '[:alnum:]' '_')_H")

  # Read the file content
  file_content=$(cat "$input_file")

  # Replace placeholders [^STR^] with their corresponding values
  file_content=$(echo "$file_content" | sed \
    -e "s/\[\^UNIX_TIME\^\]/$current_unix_time/g")

  # Escape special characters for the output
  file_content=$(echo "$file_content" | sed ':a;N;$!ba;s/"/\\"/g;s/\n/\\n/g;s/\\\\n/\\\\\\n/g')

  # Append the #define line to the output file
  echo "#define $define_name (std::string)\"$file_content\"" >> "$output_file"
done

echo "Header file $output_file generated successfully."
