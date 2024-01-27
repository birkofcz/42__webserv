#!/bin/bash

# Print Content-Type header required for CGI scripts
echo "Content-Type: text/html"
echo ""

# Read form data from standard input
read -n $CONTENT_LENGTH POST_DATA

# Parse form data
name=$(echo "$POST_DATA" | grep -oP 'name=\K\S+')

# Check if 'name' field is present in form data
if [ -n "$name" ]; then
    echo "<h1>Hello, $name!</h1>"
else
    echo "<h1>Hello, anonymous!</h1>"
fi

