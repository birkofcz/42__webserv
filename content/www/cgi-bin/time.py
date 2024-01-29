#!/usr/bin/python3

import datetime

# Generate the HTML content with the current time
html_content = """
<html>
<head>
<title>Current Time</title>
</head>
<body>
<h1>{}</h1>
</body>
</html>
""".format(datetime.datetime.now().strftime("%H:%M:%S"))

# Calculate the length of the HTML content
content_length = len(html_content.encode('utf-8'))

# Print headers
print("Content-Type: text/html")

# Print the HTML content
print(html_content)
