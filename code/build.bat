@echo off
mkdir ..\build
pushd ..\build
rc /v ..\code\icon_resources.rc
move ..\code\icon_resources.res .
cl ..\code\adbs_main.cpp ..\code\ad_bs_convert.cpp user32.lib gdi32.lib /link icon_resources.res
popd
