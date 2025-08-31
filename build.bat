@echo off
mkdir out 2> nul

setlocal enabledelayedexpansion

cd dll

set FILES=
for /R %%f in (*.c) do (
    set FILES=!FILES! "%%f"
)
gcc !FILES! -shared -Iinclude -Llib -ljvm -m64 -mwindows -std=c99 -o ../out/dll.dll > ../out/temp.txt 2>&1
for /f %%a in ('echo prompt $E ^| cmd') do set "ESC=%%a"

if %ERRORLEVEL% neq 0 (
    echo %ESC%[31mDLL compilation failed:%ESC%[0m
    type ..\out\temp.txt
) else (
    echo %ESC%[32mDLL compilation succeeded.%ESC%[0m
)
del ..\out\temp.txt

endlocal

setlocal enabledelayedexpansion

cd loader

set FILES=
for /R %%f in (*.c) do (
    set FILES=!FILES! "%%f"
)
gcc !FILES! -m64 -mwindows -std=c99 -o ../out/loader.exe > ../out/temp.txt 2>&1
for /f %%a in ('echo prompt $E ^| cmd') do set "ESC=%%a"

if %ERRORLEVEL% neq 0 (
    echo %ESC%[31mLoader compilation failed:%ESC%[0m
    type ..\out\temp.txt
) else (
    echo %ESC%[32mLoader compilation succeeded.%ESC%[0m
)
del ..\out\temp.txt

endlocal