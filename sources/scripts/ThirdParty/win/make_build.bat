cd ../../..
rmdir generated\ThirdParty /s /q
cd scripts/ThirdParty/win
cmake.exe ../..\..\ThirdParty\wxWidgets\CMakeLists.txt -B..\..\../generated\ThirdParty -G "Visual Studio 18 2026" -A x64 -DwxBUILD_STRIPPED_RELEASE=ON -DwxBUILD_SHARED=OFF -DwxBUILD_USE_STATIC_RUNTIME=ON -DwxBUILD_SAMPLES=OFF

MSBuild.exe ../..\..\generated\ThirdParty\wxWidgets.slnx /p:Configuration=Release -t:rebuild -clp:ErrorsOnly;WarningsOnly -nologo /m
set BUILD_STATUS=%ERRORLEVEL%
if %BUILD_STATUS%==0 goto Success


:Failed
echo .
echo .
echo .
echo       !!!!!!!!!!!!!!! Error !!!!!!!!!!!!!!! Build Failed !!!!!!!!!!!!!!!!!!!!
echo .
echo .
echo .
goto Exit

:Success

:Exit