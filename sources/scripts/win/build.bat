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

rmdir ..\..\generated\Linia\Linia2\resources /S /Q
mkdir ..\..\generated\Linia\Linia2\resources
robocopy ..\..\..\files\resources ..\..\generated\Linia\Linia2\resources /E /MT:32 /NFL
