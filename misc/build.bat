@echo Debug build
@echo.

@echo off

set VARS=/Od /Zi /FC /W4 /wd4130 /wd4996 /nologo

set files=..\code\sv_markdown_test.c

set options=%VARS% %files%

mkdir ..\build > nul 2> nul

pushd ..\build

cl %options%

popd
