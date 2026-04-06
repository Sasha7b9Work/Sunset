@echo off

if "%3" equ "" goto VERIFY_PARAMETER_1
if %3==edit goto VERIFY_PARAMETER_1
goto HINT

:VERIFY_PARAMETER_1
    if "%1" equ "" goto HINT
    if %1==build goto VERIFY_PARAMETER_2
    if %1==debug   ( call :MAKE %1 & goto EXIT )
    if %1==release ( call :MAKE %1 & goto EXIT )
    goto HINT

:VERIFY_PARAMETER_2
    if "%2" equ "" goto HINT
    if %2==debug   ( call :MAKE %2 & call build.bat & goto VERIFY_PARAMETER_3 )
    if %2==release ( call :MAKE %2 & call build.bat & goto VERIFY_PARAMETER_3 )
    goto HINT

:VERIFY_PARAMETER_3
    if "%3" equ "" goto EXIT
    if %3==edit ( call edit.bat & goto EXIT )
    goto HINT

:MAKE
    @echo.
    @echo %TIME%   Making Visual Studio Win Debug ...
    cd ../..
    rmdir "generated/Linia" /s /q
    cd scripts/win
    if %1==debug ( @echo on & cmake ../../CMakeLists.txt -B../../generated/Linia -G "Visual Studio 18 2026" -A x64 -DCMAKE_BUILD_TYPE=Release)
    @echo off
    if %1==release ( @echo on & cmake ../../CMakeLists.txt -B../../generated/Linia -G "Visual Studio 18 2026" -A x64 -DCMAKE_BUILD_TYPE=Release)
    @echo off
    @echo %TIME%   Complete
    exit /b

:HINT
    echo.
    echo Using:
    echo       make.bat ^<debug^|release^>
    echo       make.bat build ^<debug^|release^> [edit]
    echo.
    goto EXIT

:EXIT
