#pragma once

#if (defined (_WIN64))	//Checking if operating system is Windows 64-bit
#include "win32Platform.h"

#elif (defined (LINUX) || defined (__linux__))	//Checking if operating system is Linux
#include "linuxPlatform.h"

#endif

//Useful page: https://stackoverflow.com/questions/4605842/how-to-identify-platform-compiler-from-preprocessor-macros