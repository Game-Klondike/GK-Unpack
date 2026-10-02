@echo off
rem Builds gk-unpack.exe with MSVC: unpack.cpp + ooz (unchanged).
rem   build.cmd [path to the ooz sources]   (default: .\ooz, a copy of upstream ooz)
setlocal
set "HERE=%~dp0"
set "OOZ=%~1"
if "%OOZ%"=="" set "OOZ=%HERE%ooz"
if not exist "%OOZ%\kraken.cpp" echo ooz sources not found in "%OOZ%" & exit /b 1

rem no "if ( ... )" block here: the ")" of %ProgramFiles(x86)% would end it
where cl >nul 2>nul && goto build
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS=%%i"
if not defined VS echo MSVC (C++ build tools) not found & exit /b 1
call "%VS%\VC\Auxiliary\Build\vcvars64.bat" >nul

:build
set "OBJ=%HERE%obj"
if not exist "%OBJ%" mkdir "%OBJ%"
rem the ooz files are compiled as they are upstream; unpack.cpp includes ooz's kraken.cpp itself
cl /nologo /O2 /EHsc /MT /DNDEBUG /DWIN32 /D_CONSOLE /DUNICODE /D_UNICODE /I"%OOZ%" "%HERE%unpack.cpp" "%OOZ%\bitknit.cpp" "%OOZ%\lzna.cpp" /Fo"%OBJ%\\" /Fe"%HERE%gk-unpack.exe" || exit /b 1
echo built "%HERE%gk-unpack.exe"
