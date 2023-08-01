This text file tells us how to add a new application project to the solution directory.

1. Load up X.sln in the root dir.
2. View menu/solution explorer.
3. Right click on Solution 'X' at the top, add, new project
4. Select "Empty Project" and click next
5. Set location to X:\MyProjects\c++\2023\X\X\projects\ directory
6. Enter name of the new application and click create.
7. Copy application.cpp/h, WinMain.cpp, resource.rc, resource.h and icon.ico from the Development project into the new project's directory via explorer
8. Delete the following filters in the project within solution explorer "Header Files" and "Source Files"
9. Right click on the new project, add existing item and add those new files.
10. Move resource.h into "Resource Files"
11. Right click on the new project, "Build dependencies/project dependencies" and enable "XLib"
12. Goto application.cpp and change the name of the pWindow->setText(APPNAME);
13. Right click on the new project, "properties" and set the following settings...
14. Select "Configuration" in top left and select "Release"
15. Configuration Properties, General, output directory, = $(SolutionDir)
16. Linker, Input, Additional Dependencies, $(SolutionDir)build\x64\Release\XLib.lib;%(AdditionalDependencies)
17. Select "Configuration" in top left and select "Debug", saving changes.
18. Configuration Properties, General, output directory, = $(SolutionDir)build\$(Platform)\$(Configuration)\
19. Linker, Input, Additional Dependencies, $(SolutionDir)build\x64\Debug\XLib.lib;%(AdditionalDependencies)
20. Select "Configuration" in top left and select "All configurations", saving changes.
21. Linker, System, SubSystem to Windows
22. Configuration Properties, General, C++ Language Standard to version 20
23. Configuration Properties, General, C Language Standard to version c17
24. Configuration Properties, Debugging, Working directory to $(SolutionDir)
25. C++, general, additional include directories to $(SolutionDir)glm\;%(AdditionalIncludeDirectories)
26. Manifest Tool, input and output, DPI awareness to High DPI Aware

/*!!!!!
NOTE: Watch out for Configuration Properties, Debugging, Command. Should be set to $(TargetPath)
*/

27. Right click on the new project and select "Set as startup project"
28. Goto build menu, configuration manager and remove the Win32 platforms for both the debug and release configurations
To do this click on "Active solution platform", select edit, select x86 and click remove, then close.
Then for the new project, click on x64 platform, select edit, select win32 and click remove.

DONE.

// No need to do the below, this is how we originally got the icons into the project.
To create .ico file, use X/icons/PNG2ICO.exe
To add icons to the executable, right click on the project, Add, resource, select icon and select import, select the .ico file.
To add icon to the actual window, after having added the icon to the executable as above.
#include "resource.h" to the top of application???.cpp
then in the CApplication::initOnce() method, get a pointer to the window and call setIcon, passing the icon number found in resource.having
CWindow::getPointer()->setIcon(IDI_ICON1);