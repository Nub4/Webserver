#!/usr/bin/python3

import os

for k, v in os.environ.items():
	print (f'{k}={v}')

exit(0)