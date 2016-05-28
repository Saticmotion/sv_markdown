@echo off

set DEBUGVARS=/Od /Zi /W4 /wd4130 /wd4996
set RELEASEVARS=/O2 /Oi /W4

set files=..\code\sv_markdown_test.c

set options=%DEBUGVARS% %files%

mkdir ..\build

pushd ..\build

cl %options%

popd
