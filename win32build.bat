@ECHO "Building..."

@rmdir /s /q build
@mkdir build
@xcopy "assets" "build\assets\" /E /Y /F 
@pushd build

cl.exe /EHsc /MD  ..\src\win32_cppong.cpp /I ..\..\libs\sdl2\include /I ..\..\libs\sdl2_ttf\include /link /LIBPATH:..\..\libs\sdl2\lib\x86 SDL2.lib SDL2main.lib /LIBPATH:..\..\libs\sdl2_ttf\lib\x86 SDL2_ttf.lib /SUBSYSTEM:CONSOLE

@copy ..\..\libs\sdl2_ttf\lib\x86\*.dll .
@copy ..\..\libs\sdl2\lib\x86\*.dll .

@popd
