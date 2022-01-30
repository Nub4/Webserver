#! /usr/bin/python3

import os

content = ""
for k, v in os.environ.items():
	content += f"{k}={v}\n"

header= "HTTP/1.1 200 OK\r\n"
header+= "Cache-Control: no-cache, private\r\n"
header+= "Content-Type: text/plain\r\n"
header+= "Content-Length: " + str(len(content)) + "\r\n\n"

print (header)
print (content)

exit(0)