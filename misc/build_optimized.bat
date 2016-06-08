@echo off

set VARS=/O2 /Zi /Oi /W4 /wd4996

set files=..\code\sv_markdown_test.c

set options=%VARS% %files%

mkdir ..\build

pushd ..\build

cl %options%

popd
