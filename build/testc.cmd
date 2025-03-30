@echo off
rem set PLATFORM=0
rem set SOFTDISK=0
set SUFFIX=%PLATFORM%%SOFTDISK%

set KERNEL=kernelstart_%SUFFIX%.hex

set COMMON=..\..\atari-common
set FAT16=..\..\atari-fat16
set TOOLS=..\..\atari-tools

set WDC=%TOOLS%\WDC\bin
set INC=%TOOLS%\WDC\include
set WDC_LIB=%TOOLS%\WDC\lib

set MODEL=S

PATH=%PATH%;%WDC%

set SRC=..\src
set BIN=..\bin
set REL=..\release
set RES=..\res

call :compile_module testc
if NOT %result%==0 goto error


pushd %REL%
mkdir obj > nul 2> nul
mkdir lst > nul 2> nul

move %SRC%\*.lst lst > nul
move %SRC%\*.obj obj > nul

%WDC%\wdcln -V -T -O testc.hex -HI obj\testc -Lc%MODEL%
%WDC%\wdcln -V -T -O testc.bin -HB obj\testc -Lc%MODEL%

popd

pause
goto :eof

:error
pause
goto :eof

rem ------------------------------------------------------------
rem
rem ------------------------------------------------------------
:docopy

  echo %file%
  for /F %%i in (%file%) do (
    if NOT %%i==:00000001FF  echo %%i >> %KERNEL%
  )
  
  goto :eof
  

rem ------------------------------------------------------------
rem
rem ------------------------------------------------------------
:compile_module

	pushd %SRC%
	
	echo *****************************************************************
	echo compiling %1.c
	echo *****************************************************************

	wdc816cc -M%MODEL% -LT -SOP %1.c -I %INC%
	set result=%ERRORLEVEL%
	popd
	