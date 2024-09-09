@echo off

mkdir ..\build
pushd ..\build

set "sourceDir=C:\Users\James\documents\learning\graphics\3D-Graphics-Programming-From-Scratch\code"

cl /Zi /Fe"graphics_from_scratch" ..\code\*.c /I C:\Users\James\SDL2\SDL2-2.30.5\include /I "%sourceDir%" /link /LIBPATH:c:\users\james\sdl2\sdl2-2.30.5\lib\x64 SDL2main.lib SDL2.lib SDL2_image.lib shell32.lib /LIBPATH:"%sourceDir%" /SUBSYSTEM:CONSOLE

popd
