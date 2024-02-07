#!/usr/bin/python3
import cgi

print("Status: 418 I'm a teapot")
print("Content-Type: text/html")
print()  # End of headers

# ASCII art for the coffee machine
ascii_coffee_machine = """
<pre>
         ( (
          ) )
       ........
       |      |]
       \      /   
        `----'
</pre>
"""

html_content = f"""
<html>
<head>
<title>Coffee Machine</title>
</head>
<body>
<h1>Coffee Machine</h1>
{ascii_coffee_machine}
<p>I'm a teapot!</p>
</body>
</html>
"""

print(html_content)

