:: Run CMake commands
mkdir .\build
cd .\build || exit /b 1
cmake -DCMAKE_BUILD_TYPE=Debug .. || exit /b 1
cmake --build . || exit /b 1

pause