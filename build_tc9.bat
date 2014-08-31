@echo off
:: ===============================================================================
::                 Copyright Siemens PLM Software 2014. 
::                  Unpublished - All rights reserved
:: ===============================================================================
:: File			: build_env.bat
:: Description		: Script for setting up build environment
:: ===============================================================================
::  Developer            Date            Description
::  Mattias Jonsson      2014-04-14      Initial Version
:: ===============================================================================

set MSDEV_HOME=C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC
set TC_ROOT=D:\App\Teamcenter_9_1
set TC_DATA=D:\Data\Teamcenter_9_1\tcdata

:: DO NOT EDIT BELOW THIS LINE
:: Set to target platform (x86 or x64)
set TC_PLATFORM=x64
set TC_VERSION=TC_9
set TC_USER_BIN=%TC_ROOT%\bin
set TC_USER_LIB=%TC_ROOT%\lib

:: Do not edit below this line!
set BINARIES=%~dp0\Binaries
if not exist "%BINARIES%\make.exe" goto nognubin
if not exist "%MSDEV_HOME%\vcvarsall.bat" goto nomsdev
if not exist "%TC_ROOT%\bin\tcserver.exe" goto notcroot
if not exist "%TC_DATA%\tc_profilevars.bat" goto notcdata

echo Build Environment is OK!
echo.

set PATH=%BINARIES%;%PATH%
call "%MSDEV_HOME%\vcvarsall" %TC_PLATFORM% >NUL
call "%TC_DATA%\tc_profilevars" >NUL
:: set INCLUDE=%INCLUDE%;%TC_INCLUDE%

goto end

:nognubin
echo Build Environment is NOT OK!
echo.
echo Could not find make environment at "%BINARIES%".
echo.
goto error_end

:nomsdev
echo Build Environment is NOT OK!
echo.
echo Could not find Visual Studio installation at "%MSDEV_HOME%".
echo.
goto error_end

:notcroot
echo Build Environment is NOT OK!
echo.
echo Could not find Teamcenter installation at "%TC_ROOT%".
echo.
goto error_end

:notcdata
echo Build Environment is NOT OK!
echo.
echo Could not find Teamcenter Data folder at "%TC_DATA%".
echo.
goto error_end

:error_end
cd /d %~dp0
set BINARIES=
cmd

:end
cd /d %~dp0
set BINARIES=
if [%1]==[] cmd
if [%1]==[dev] devenv
if [%1]==[all] make all
if [%1]==[install] make install