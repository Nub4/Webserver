#! /usr/bin/python3

import os

def getHTMLpage():
	page =	"<!DOCTYPE html>" \
			"<html>" \
				"<style>" \
					"#foo {" \
						"position: fixed;" \
						"top: 25px;" \
						"left: 100px;" \
					"}" \
				"</style>" \
				'<body style="background-color:powderblue;">' \
					'<div id="foo">' \
						'<h1>This html page was created by a CGI!</h1>' \
					'</div>' \
				'</body>' \
			'</html>'
	return page


content = ""

try:
	f = open("./www/created_file.html", "x")
	f.close()
	f = open("./www/created_file.html", "w")
	f.write(getHTMLpage())
	content = "File created!"
except:
	content = "File already exists!"

header= "HTTP/1.1 200 OK\r\n"
header+= "Cache-Control: no-cache, private\r\n"
header+= "Content-Type: text/plain\r\n"
header+= "Content-Length: " + str(len(content)) + "\r\n"


print (header)
print (content)

exit(0)
