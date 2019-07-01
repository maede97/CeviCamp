cd build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE
mingw32-make.exe || goto :error
cd ..
mkdir .\release\res\
copy .\build\main.exe .\release\CeviCamp.exe
robocopy .\res\ .\release\res\ /s /e
copy /y .\build\openal32.dll .\release\
"C:\Program Files\7-Zip\7z.exe" a -tzip release.zip .\release\*
rd /s /q .\release\
goto :EOF

:error
echo Failed with error %errorlevel%
exit /b %errorlevel%