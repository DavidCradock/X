This text file tells us how to add a new application project to the solution directory.

1. Load up X.sln in the root dir.
2. View menu/solution explorer.
3. Right click on Solution 'X' at the top, add, new project
4. Select "Empty Project" and click next
5. Set location to X\projects directory
6. Enter name of the new application and click create.
7. Copy application.cpp/h and WinMain.cpp from the Development project into the new project's directory via explorer
8. Right click on the new project, add existing item and add those three files.
9. Right click on the new project, "Build dependencies/project dependencies" and enable "XLib"
10. Right click on the new project, "properties" and set the following settings...

Select "Configuration" in top left and select "Release"
Configuration Properties, General, output directory, = $(SolutionDir)
Linker, Input, Additional Dependencies, $(SolutionDir)build\x64\Release\XLib.lib;%(AdditionalDependencies)

Select "Configuration" in top left and select "Debug"
Configuration Properties, General, output directory, = $(SolutionDir)build\$(Platform)\$(Configuration)\
Linker, Input, Additional Dependencies, $(SolutionDir)build\x64\Debug\XLib.lib;%(AdditionalDependencies)

Select "Configuration" in top left and select "All configurations"
Linker, System, SubSystem to Windows
Configuration Properties, General, C++ Language Standard to version 20
Configuration Properties, General, C Language Standard to version c17
Configuration Properties, Debugging, Working directory to $(SolutionDir)
C++, general, additional include directories to $(SolutionDir)glm\;%(AdditionalIncludeDirectories)
Manifest Tool, input and output, DPI awareness to High DPI Aware

11. Right click on the new project and select "Set as startup project"
12. Goto build menu, configuration manager and remove the Win32 platforms for both the debug and release configurations
