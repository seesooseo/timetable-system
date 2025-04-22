@echo off
setlocal enabledelayedexpansion

rem ──────────────── Configuration ────────────────
rem  Max size per output file in bytes (512 MB)
set MAXSIZE=536870912

rem  Initial part index and current size counter
set PART=1
set CURSIZE=0

rem  Remove any previous output parts
del combined_part*.txt 2>nul

rem ──────────────── Loop over source files ────────────────
for /R %%f in (*.cpp *.hpp *.json *.cmake CMakeLists.txt) do (
    rem  Get size of the next file
    set FILESIZE=%%~zf

    rem  If adding this file would exceed MAXSIZE, start a new part
    set /A NEXTSIZE=!CURSIZE! + !FILESIZE!
    if !NEXTSIZE! GTR !MAXSIZE! (
        set /A PART+=1
        set CURSIZE=0
    )

    rem  Build the current output filename
    set OUTFILE=combined_part!PART!.txt

    rem  Append a header, the file’s contents, and a blank line
    >> "!OUTFILE!" echo --- %%f ---
    >> "!OUTFILE!" type "%%f"
    >> "!OUTFILE!" echo.

    rem  Update our running total
    set /A CURSIZE+=!FILESIZE!
)

echo.
echo Done. Created !PART! part(s), each up to %MAXSIZE% bytes.
