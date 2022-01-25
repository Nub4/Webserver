#!/usr/local/bin/python3

import os

content = ""
for k, v in os.environ.items():
	content += f"{k}={v}\n"

toParse = ""
toParse = os.environ["QUERY_STRING"]

input0 = toParse[: toParse.find('&')]
input0 = input0[toParse.find('=')+1 :]
input1 = toParse[toParse.find('&')+1 :]
input1 = input1[toParse.find('=')+1 :]

result = str(int(input0) + int(input1))
content += input0 + input1


header= "HTTP/1.1 200 OK\r\n"
header+= "Cache-Control: no-cache, private\r\n"
header+= "Content-Type: text/plain\r\n"
header+= "Content-Length: " + str(len(content)) + "\r\n\n"
print (header)
print (content)

exit(0)