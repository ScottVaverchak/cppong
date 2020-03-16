rmdir /s /q build
mkdir build
xcopy "assets" "build\assets\" /E /Y /F 
pushd build

cl.exe /EHsc /MD  ..\src\win32_cppong.cpp /I ..\libs\win32\sdl2\include /I ..\libs\win32\sdl2_ttf\include /link /LIBPATH:..\libs\win32\sdl2\lib\x86 SDL2.lib SDL2main.lib /LIBPATH:..\libs\win32\sdl2_ttf\lib\x86 SDL2_ttf.lib /SUBSYSTEM:CONSOLE

copy ..\libs\win32\sdl2_ttf\lib\x86\*.dll .
copy ..\libs\win32\sdl2\lib\x86\*.dll .

copy ..\uni0553-webfont.ttf .

popd
