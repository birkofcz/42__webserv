#!/bin/bash

# Set the content type to HTML
echo "Content-type: text/html"
echo ""

# Check if the request method is POST
if [ "$REQUEST_METHOD" = "POST" ]; then
    # Get the content length from the HTTP headers
    CONTENT_LENGTH=${CONTENT_LENGTH:-0}

    # Read the POST data (file content) from standard input
    read -n $CONTENT_LENGTH FILE_CONTENT

    # Parse the content-disposition header to extract the filename
    FILENAME=$(echo "$HTTP_CONTENT_DISPOSITION" | grep -oP 'filename="\K[^"]+')

    # If the filename is empty, use a default name
    if [ -z "$FILENAME" ]; then
        FILENAME="uploaded_file"
    fi

    # Determine the file extension
    FILE_EXTENSION="${FILENAME##*.}"

    # Generate a unique filename to avoid overwriting existing files
    UNIQUE_FILENAME="$(date +"%Y%m%d%H%M%S")_$RANDOM.$FILE_EXTENSION"

    # Store the file with the original filename and extension
    echo "$FILE_CONTENT" > "content/www/testloc1/uploads/$UNIQUE_FILENAME"

    # Respond with a success message
    echo "<h1>Upload Successful</h1>"
    echo "<p>The file '$FILENAME' has been successfully uploaded as '$UNIQUE_FILENAME'.</p>"
else
    # Respond with an error message if the request method is not POST
    echo "<h1>Error: Method Not Allowed</h1>"
    echo "<p>This script only accepts POST requests.</p>"
fi
