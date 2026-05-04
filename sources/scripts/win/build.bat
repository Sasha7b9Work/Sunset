@echo off

@echo.
@echo %TIME%   Building VS-GUI version ...

MSBuild.exe ..\..\generated\Linia\Sunset.slnx -clp:ErrorsOnly;WarningsOnly -nologo /m
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Succsess

:Failed
@echo %TIME%   !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!
goto Exit

:Succsess
@echo %TIME%   Complete

:Exit
