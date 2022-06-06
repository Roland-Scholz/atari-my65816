@echo off

set REL=..\release
rmdir /S /Q %REL%
mkdir %REL%

echo ============================================================
echo = kernel FPGA NOSOFTDISK
echo ============================================================
set PLATFORM=0
set SOFTDISK=0
call kernel.cmd

echo ============================================================
echo = kernel FPGA SOFTDISK
echo ============================================================
set SOFTDISK=1
call kernel.cmd
pause
