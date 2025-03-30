@echo off
set BUILD_DIR=build
set OUTPUT_DIR=output

:: Create build directory if it doesn't exist
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

:: Run CMake
echo Generating build files...
cmake .. -G "Visual Studio 17 2022"

:: Compile
echo Building project...
cmake --build . --config Release

:: Return to root directory
cd ..


:: Ensure the output directory exists
if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%

:: Prompt for filename
set /p FILENAME=Enter filename name (e.g. tracer):

:: Optional: Run the raytracer
echo Running RayTracer...
bin\raytracing.exe > %OUTPUT_DIR%\%FILENAME%.ppm
