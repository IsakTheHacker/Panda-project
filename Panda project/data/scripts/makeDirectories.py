#Imports
import os

#Checking if data directory exists
if not os.path.isdir("data"):
	try:
		os.mkdir("data")
	except OSError:
		print("Creation of 'data' directory failed!")

#Checking if screenshots directory exists
if not os.path.isdir("screenshots"):
	try:
		os.mkdir("screenshots")
	except OSError:
		print("Creation of 'screenshots' directory failed!")

#Checking if scripts directory exists
if not os.path.isdir("scripts"):
	try:
		os.mkdir("scripts")
	except OSError:
		print("Creation of 'scripts' directory failed!")

	if not os.path.isfile("scripts/standard_startscript.txt"):
		lines = [
			"print(\"Hello World\")"
		]
		try:
			with open("scripts/standard_startscript.txt","w") as file:
				newLine = "\n"
				for item in lines:
					if item == lines[len(lines)-1]:
						newLine = ""
					file.write(item + newLine)
		except OSError:
			print("Creation of 'standard_startscript.txt' file failed!")