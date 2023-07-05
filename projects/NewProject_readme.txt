This text file tells us how to add a new application project to the solution directory.

1. Load up X.sln in the root dir.
2. View menu/solution explorer.
3. Right click on Solution 'X' at the top, add, new project
4. Select "Empty Project" and click next
5. Set location to X\projects directory
6. Enter name of the new application and click create.
7. Copy application.cpp/h, WinMain.cpp, resource.rc, resource.h and icon.ico from the Development project into the new project's directory via explorer
8. Right click on the new project, add existing item and add those new files.
9. Right click on the new project, "Build dependencies/project dependencies" and enable "XLib"
10. Right click on the new project, "properties" and set the following settings...

Select "Configuration" in top left and select "Release"
Configuration Properties, General, output directory, = $(SolutionDir)
Linker, Input, Additional Dependencies, $(SolutionDir)build\x64\Release\XLib.lib;%(AdditionalDependencies)

Select "Configuration" in top left and select "Debug", saving changes.
Configuration Properties, General, output directory, = $(SolutionDir)build\$(Platform)\$(Configuration)\
Linker, Input, Additional Dependencies, $(SolutionDir)build\x64\Debug\XLib.lib;%(AdditionalDependencies)

Select "Configuration" in top left and select "All configurations", saving changes.
Linker, System, SubSystem to Windows
Configuration Properties, General, C++ Language Standard to version 20
Configuration Properties, General, C Language Standard to version c17
Configuration Properties, Debugging, Working directory to $(SolutionDir)
C++, general, additional include directories to $(SolutionDir)glm\;%(AdditionalIncludeDirectories)
Manifest Tool, input and output, DPI awareness to High DPI Aware

11. Right click on the new project and select "Set as startup project"
12. Goto build menu, configuration manager and remove the Win32 platforms for both the debug and release configurations

DONE.

// No need to do the below, this is how we originally got the icons into the project.
To create .ico file, use X/icons/PNG2ICO.exe
To add icons to the executable, right click on the project, Add, resource, select icon and select import, select the .ico file.
To add icon to the actual window, after having added the icon to the executable as above.
#include "resource.h" to the top of application???.cpp
then in the CApplication::initOnce() method, get a pointer to the window and call setIcon, passing the icon number found in resource.having
CWindow::getPointer()->setIcon(IDI_ICON1);