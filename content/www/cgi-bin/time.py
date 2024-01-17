#!/usr/bin/python3

import datetime
import os

# Print HTTP headers
print("Content-type: text/html")
print()

# Print the current time
now = datetime.datetime.now()
print("<html><body>")
print("<h1>Current Time</h1>")
print("<p>The current time is: {}</p>".format(now.strftime("%Y-%m-%d %H:%M:%S")))
print("</body></html>")
