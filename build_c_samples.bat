:: Build all C samples
:: The library must be compiled with ENABLE_MEGAWIFI set to 1

ECHO off
SET "GDKPLUS=%cd:\=/%"
SET "GDKPLUS_WIN=%cd%"
SET "XGCC_WIN=%GDKPLUS_WIN%\x68k-gcc\bin"
SET "PATH=%GDKPLUS_WIN%\bin;%XGCC_WIN%;%PATH%"

set DIRS=sample\basics\hello-world ^
    sample\basics\image ^
    sample\bench ^
    sample\bitmap\cube3D ^
    sample\bitmap\partic ^
    sample\demo\bad-apple ^
    sample\demo\starfield-donut ^
    sample\fx\h-int\wobble ^
    sample\fx\hilight-shadow ^
    sample\fx\scroll\linescroll ^
    sample\joy-test ^
    sample\megawifi ^
    sample\multitasking ^
    sample\sonic ^
    sample\sound ^
    sample\xgm-player*

for /D %%i in (%DIRS%) do (
    echo "%%i"
    %GDKPLUS_WIN%\bin\make -C "%%i" release -f %GDKPLUS_WIN%\makefile.gen
    %GDKPLUS_WIN%\bin\make -C "%%i" distclean -f %GDKPLUS_WIN%\makefile.gen 
)  
