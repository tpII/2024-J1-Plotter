#!/bin/bash

# Directory to start searching from (current directory by default)
start_dir=${1:-.}

# Output file to store aggregated content (optional)
output_file="all_terraform_files.tf"

# Clear the output file if it exists
> "$output_file"

# Find and concatenate all .tf files into the output file
find "$start_dir" -type f -name "*.tf" -exec cat {} + >> "$output_file"

echo "All Terraform files aggregated into $output_file."
