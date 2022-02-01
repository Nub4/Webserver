#! /usr/local/bin/python3

import os

content = ""

toParse = ""
toParse = os.environ["QUERY_STRING"]

input0 = toParse[: toParse.find('&')]
input0 = input0[toParse.find('=')+1 :]
input1 = toParse[toParse.find('&')+1 :]
input1 = input1[toParse.find('=')+1 :]
try:
	int(input0)
	int(input1)
except ValueError:
	content += "'" + input0 + "' and '" + input1 + "' don't add up!"
else:
	content += input0 + " + " + input1 + " = " + str(int(input0) + int(input1))

header= "HTTP/1.1 200 OK\r\n"
header+= "Cache-Control: no-cache, private\r\n"
header+= "Content-Type: text/plain\r\n"
header+= "Content-Length: " + str(len(content)) + "\r\n"


print (header)
print (content)

exit(0)