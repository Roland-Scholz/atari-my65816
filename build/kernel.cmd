@echo off
rem set PLATFORM=0
rem set SOFTDISK=0
set SUFFIX=%PLATFORM%%SOFTDISK%

set KERNEL=kernelstart_%SUFFIX%.hex

set COMMON=..\..\atari-common
set FAT16=..\..\atari-fat16
set TOOLS=..\..\atari-tools

set CC65=%TOOLS%\cc65\bin
set SRC=..\src
set BIN=..\bin
set REL=..\release
set RES=..\res

call :compile_module kernel 0000 65816 1FE4
if NOT %result%==0 goto error

call :compile_module myos 0000 65816 2000
if NOT %result%==0 goto error

call :compile_module start 1FE4 65816 001C
if NOT %result%==0 goto error

%CC65%\bin2hex %RES%\Bm437_IBM_BIOS1.bin %REL%\font.hex -o 0000

pushd %REL%

mkdir obj > nul 2> nul
mkdir lst > nul 2> nul

move %REL%\*.lst lst > nul
move *.o obj > nul
move *.a obj > nul

set file=kernel_%SUFFIX%.hex
call :docopy
set file=start_%SUFFIX%.hex
call :docopy
echo :00000001FF >> %KERNEL%

popd
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

%CC65%\ca65 --cpu %3 -DPLATFORM=%PLATFORM% -DSOFTDISK=%SOFTDISK% -l %REL%\%1.lst %SRC%\%1.a65 -I ..\inc -I %COMMON%\inc -I %COMMON%\src -I %FAT16%\inc -I %FAT16%\src -o %REL%\%1_%SUFFIX%.o

set result=%ERRORLEVEL%

if %result%==0 (

	%CC65%\ld65 -C homebrew.cfg %REL%\%1_%SUFFIX%.o -o %REL%\%1_%SUFFIX%.a
	java -jar %TOOLS%\Obj2Com\jar\ObjUtil.jar ObjFill %REL%\%1_%SUFFIX%.a %4
	%CC65%\bin2hex %REL%\%1_%SUFFIX%.a %REL%\%1_%SUFFIX%.hex -o %2
	
	rem java -jar %TOOLS%\Obj2Com\jar\ObjUtil.jar Obj2Com %REL%\%1.a %2
	
)
