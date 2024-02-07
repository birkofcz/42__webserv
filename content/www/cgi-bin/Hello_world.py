#!/usr/bin/python3

# Generate the HTML content with the specified title
html_content = """
<html>
<head>
<title>This is a CGI page written in python</title>
<style>
    h1 { text-align: center; }
</style>
</head>
<body>
<h1>Hello world! This is a CGI page written in python</h1>
</body>
</html>
"""

# Calculate the length of the HTML content
content_length = len(html_content.encode('utf-8'))

# Print headers
print("Content-Type: text/html")
print()  # Separate headers from the body

# Print the HTML content
print(html_content)
