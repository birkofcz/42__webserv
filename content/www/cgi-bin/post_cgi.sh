#!/bin/bash

# Print Content-Type header required for CGI scripts
echo "Content-Type: text/html"
echo ""

# Infinite loop for error testing
# while true; do

	# Read form data from standard input
	read -n $CONTENT_LENGTH POST_DATA

	# Parse form data
	name=$(echo "$POST_DATA" | grep -oP 'name=\K\S+')

	# Check if 'name' field is present in form data
	if [ -n "$name" ]; then
		echo -e "<h1>Hello, $name!</h1>\r\n"
	else
		echo -e "<h1>Hello, anonymous!</h1>\r\n"
	fi

# done  # This closes the while loop