mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -G "MinGW Makefiles"
mingw32-make.exe || goto :error
cd ..
START ./build/main.exe
goto :EOF

:error
echo Failed with error %errorlevel%
exit /b %errorlevel% 