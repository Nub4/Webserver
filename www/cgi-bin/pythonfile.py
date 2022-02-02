#! /usr/local/bin/python3

# import os

# content = ""

# toParse = ""
# toParse = os.environ["QUERY_STRING"]

# input0 = toParse[: toParse.find('&')]
# input0 = input0[toParse.find('=')+1 :]

message2 = 'Content-type: test/html\r\n'
print(message2)

# fn = os.path.basename(input0)
# open(fn, "wb").write(input0.read())

message = 'The file was uploaded successfully'
print (message)


# import os

# content = ""

# toParse = ""
# toParse = os.environ["QUERY_STRING"]

# input0 = toParse[: toParse.find('&')]
# input0 = input0[toParse.find('=')+1 :]


# # fn = os.path.basename(input0)
# # open(fn, "wb").write(input0.read())

# content = input0

# header= "HTTP/1.1 200 OK\r\n"
# header+= "Cache-Control: no-cache, private\r\n"
# header+= "Content-Type: text/html\r\n"
# header+= "Content-Length: " + str(len(content)) + "\r\n"

# print (header)
# print (content)

# exit(0)