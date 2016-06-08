@echo Optimized build
@echo.
@echo off

set VARS=/O2 /Zi /Oi /FC /W4 /wd4996 /nologo

set files=..\code\sv_markdown_test.c

set options=%VARS% %files%

mkdir ..\build > nul 2> nul

pushd ..\build

cl %options%

popd
