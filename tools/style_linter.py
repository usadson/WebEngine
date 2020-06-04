#!/usr/bin/python3
#
# Copyright (C) 2020 Tristan. All Rights Reserved.
# This file is licensed under the BSD 2-Clause license.
# See the COPYING file for licensing information.

import os
import sys

class Colors:
	BLUE = "\033[34m"
	RED = "\033[31m"
	RESET = "\033[0m"
	YELLOW = "\033[33m"

class Characters:
	SPACE = 0x20
	TABULATOR = 0x9

fileNames = None
totalErrors = 0
totalErrorFiles = 0

if len(sys.argv) == 1:
	fileNames = [os.path.join(dp, f) for dp, dn, filenames in os.walk(".")
			for f in filenames if os.path.splitext(f)[1] == '.c'
							   or os.path.splitext(f)[1] == '.h'
							   or os.path.splitext(f)[1] == '.cpp'
							   or os.path.splitext(f)[1] == '.hpp']
else:
	fileNames = sys.argv[1:]

for fileName in fileNames:
	try:
		fd = open(fileName)
	except (IOError, OSError) as e:
		print("%s%s" % (Colors.RED, e))
		print("Failed to open that file.%s" % Colors.RESET)
		sys.exit()

	with fd:
		linenr = 0
		errors = 0
		for line in fd:
			linenr += 1
			count = 0
			buf = [*line]
			for char in buf:
				if ord(char) == Characters.TABULATOR:
					count += 4 - (count % 4)
				elif char != '\n':
					count += 1
			# Line length checking
#			if count > 80:
#				errors += 1
#				print("Line %s%s:%i%s is too long: (%s%i%s characters)\n%s%s%s"
#				% (
#					Colors.BLUE, fileName, linenr, Colors.RESET,
#					Colors.BLUE, count, Colors.RESET,
#					Colors.YELLOW, line.strip(), Colors.RESET
#				))
			# End-of-line checking
			if len(buf) > 1:
				if ord(buf[-2]) == Characters.TABULATOR:
					errors += 1
					print("Line %s%s:%i%s ends with a tabulator"
						% (Colors.BLUE, fileName, linenr, Colors.RESET))
				elif ord(buf[-2]) == Characters.SPACE:
					errors += 1
					print("Line %s%s:%i%s ends with a space"
						% (Colors.BLUE, fileName, linenr, Colors.RESET))
		totalErrors += errors
		if errors == 1:
			totalErrorFiles += 1
			print("File '%s' has 1 error" % fileName)
		elif errors > 1:
			totalErrorFiles += 1
			print("File '%s' has %i error(s)" % (fileName, errors))

if totalErrors == 0:
	print("No errors found!")
else:
	print("%s%i%s error%s in %s%i%s file%s." % (
		Colors.BLUE, totalErrors, Colors.RESET,
		"" if totalErrors == 1 else "s",
		Colors.BLUE, totalErrorFiles, Colors.RESET,
		"" if totalErrorFiles == 1 else "s"
	))
