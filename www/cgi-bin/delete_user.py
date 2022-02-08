#! /usr/local/bin/python3

import os

content = ""

toParse = ""
toParse = os.environ["QUERY_STRING"]

input0 = toParse[: toParse.find('&')]
input0 = input0[toParse.find('=')+1 :]

mark = 0
mark2 = 0

r = open("./assets/feedback_log.txt", "r")
lines = r.readlines()
r.close()

f = open("./assets/feedback_log.txt", "w")
for line in lines:
    if input0 in line:
        mark = 1
        line2 = line
    else:
        mark2 = 1
        f.write(line)

if mark2 == 0:
    f.write(line2)

f.close()


f = open("./assets/feedback_log.txt", "r")

content += "<!DOCKTYPE html>"
content += "<html>"
content +=  "<style>"
content +=      "#foo { position: fixed;"
content +=      "top: 25px;"
content +=      "color: white;"
content +=      "left: 50px; }"
content +=      "body { background-image: url('../randomfiles/photo.jpeg');"
content +=      "background-repeat: no-repeat;"
content +=      "background-attachment: fixed;"
content +=      "background-size: cover; }"
content +=  "</style>"
content +=  "<body style=" + '"background-color:black;"' + ">"
content +=      '<div id="foo">'
content +=          "<h1>Feedbacks:</h1><br>"

for x in f:
    s = x.split(" & ")
    count = 0
    for y in s:
        if count == 0 & mark == 0:
            content += y + ":<br>"
        else:
            content += y + "<br><br>"
        count += 1
f.close()

content +=      "</div>"
content +=      '<img src="19.jpeg>'
content +=  "</body>"
content += "</html>"

header= "HTTP/1.1 200 OK\r\n"
header+= "Cache-Control: no-cache, private\r\n"
header+= "Content-Type: text/html\r\n"
header+= "Content-Length: " + str(len(content)) + "\r\n"

print (header)
print (content)

exit(0)