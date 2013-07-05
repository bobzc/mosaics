#!/usr/bin/python

import glob
import os
import sys

print str(sys.argv)

# check arguements
if len(sys.argv) <= 1:
	print "Invalid call!"
	exit(0)

# get directory name
dir_name = sys.argv[1]
os.chdir(dir_name)

# import images
print "Importing " + dir_name + "..."
for fname in glob.glob("*.jpg"):
	print "Importing " + fname + "..."
	os.system("../import_img.py " + fname)
