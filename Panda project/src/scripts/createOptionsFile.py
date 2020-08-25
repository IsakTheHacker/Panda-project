#Imports
import os

#Checking if data directory exists
if not os.path.isdir("data"):
	try:
		os.mkdir("data")
	except OSError:
		print("Creation of 'data' directory failed!")

if not os.path.isfile("data/options.txt"):
	options = [
		"force_x=4",
		"force_y=4",
		"camera_x_speed=5",
		"camera_y_speed=5",
		"x_speed=1",
		"y_speed=1",
		"z_speed=1",
		"hidden_cursor=1",
		"close_console_without_input=1",
		"lower_border=-100",
		"upper_border=0"
	]
	try:
		with open("data/options.txt","w") as optionsFile:
			newLine = "\n"
			for item in options:
				if item == options[len(options)-1]:
					newLine = ""
				optionsFile.write(item + newLine)
	except OSError:
		print("Creation of 'options.txt' file failed!")
print("It works")