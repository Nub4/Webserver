#!/usr/bin/python3
import  os


# Get filename here.
fileitem = os.environ["QUERY_STRING"]
# Test if the file was uploaded
if fileitem:
   # strip leading path from file name to avoid
   # directory traversal attacks
   fn = os.path.basename(fileitem.filename.replace("\\", "/" ))
   open('/tmp/' + fn, 'wb').write(fileitem.file.read())
   message = 'The file "' + fn + '" was uploaded successfully'
else:
   message = 'No file was uploaded'


# import os
 
# fileitem = os.environ["QUERY_STRING"]
 
# # check if the file has been uploaded
# if fileitem.filename:
#     # strip the leading path from the file name
#     fn = os.path.basename(fileitem.filename)
     
#    # open read and write the file into the server
#     open(fn, 'wb').write(fileitem.file.read())

exit(0)