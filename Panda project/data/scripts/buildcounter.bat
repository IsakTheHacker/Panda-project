@echo off
for /f "tokens=*" %%s in (numberofbuilds.txt) do (
	set myFile=%%s
)
set /a var=%myFile% + 1
@echo %var% > numberofbuilds.txt