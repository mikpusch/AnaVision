# AnaVision
FLIM/FRET and other image analysis

If you just want to use AnaVision, download the following folder: Source/AnaVision/Release and execute AnaVision.exe. This works only on Windows. It may be that you have to install a suitable VC++ runtime environment. See for example here: https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170

In case you want to build AnaVision by yourself follow these steps:

AnaVision was built using Microsoft Visual Studio 2022.

Download the source-folder and use the project file "AnaVision.vcxproj" found in the AnaVision folder to load the project into VisualStudio.

The only adjustment to be made is the following: Replace "Complex.h" in the include folder of Visual Studio by the file "Complex.h" file in the github source folder.

