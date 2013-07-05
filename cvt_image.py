#!/usr/bin/python

import glob
import os
import math

i = 1
for files in glob.glob("*"):
	print "Converting file #", i, ": ", files, "..."
	for x in range(1, 6):
		width = str(int(math.pow(2, x - 1) * 16))
		size = width + "x" + width
		os.system("convert -define jpeg:size=" +  size + " " + files + " -thumbnail " + size + "^ -gravity center -extent " + size + " " + width + "/" + files)
	i += 1
