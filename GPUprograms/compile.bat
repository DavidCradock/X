@echo off
echo Compiling shader programs to .spv files...
glslc.exe dev.vert -o dev_v.spv
glslc.exe dev.frag -o dev_f.spv
glslc.exe default.vert -o default_v.spv
glslc.exe default.frag -o default_f.spv
echo Done.
pause