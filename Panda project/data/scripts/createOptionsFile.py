#Imports
import os

#Checking if options file exists
if not os.path.isfile("data/options.txt"):
	options = [
		"force_x=4",
		"force_y=4",
		"camera_x_speed=5",
		"camera_y_speed=5",
		"x_speed=0.2",
		"y_speed=0.2",
		"z_speed=0.2",
		"hidden_cursor=1",
		"close_console_without_input=1",
		"lower_border=0",
		"upper_border=none",
		"startscript=scripts/standard_startscript.txt",
		"console-title=The Panda Project",
		"console-heading=The Panda Project",
		"window-title=The Panda Project",
		"model-format=egg",
		"hide_fps=0",
		"pitch-behavior_while_rotating=none",
		"sneak-distance=0.5",
		"fov=70"
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

#Checking if scripting options file exists
if not os.path.isfile("data/scripting_options.txt"):
	scripting_options = [
		"cin_enabled=1"
	]
	try:
		with open("data/scripting_options.txt","w") as optionsFile:
			newLine = "\n"
			for item in scripting_options:
				if item == scripting_options[len(scripting_options)-1]:
					newLine = ""
				optionsFile.write(item + newLine)
	except OSError:
		print("Creation of 'scripting_options.txt' file failed!")