@echo off
rem One-Click Windows Installation for 3D Model Painter

echo === 3D Model Painter: Easy Windows Installation ===
echo Setting up your 3D Model Painter application...

rem Check if g++ is available
where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo ! g++ compiler not found.
    echo Please install MinGW or another C++ compiler and add it to your PATH.
    echo You can download it from: https://www.mingw-w64.org/downloads/
    pause
    exit /b 1
)

rem Compile Enhanced Fun Model Painter
echo - Compiling Enhanced Fun Model Painter...
g++ -std=c++17 EnhancedFunModelPainter.cpp -o EnhancedFunModelPainter.exe
if %ERRORLEVEL% neq 0 (
    echo ! Compilation failed. Trying simpler version...
) else (
    echo + Successfully compiled EnhancedFunModelPainter.exe
)

rem Compile Fun Model Painter as backup
echo - Compiling Fun Model Painter...
g++ -std=c++17 FunModelPainter.cpp -o FunModelPainter.exe
if %ERRORLEVEL% neq 0 (
    echo ! Compilation failed for FunModelPainter.
) else (
    echo + Successfully compiled FunModelPainter.exe
)

rem Create a launcher batch file
echo @echo off > Run_3D_Model_Painter.bat
echo cd "%%~dp0" >> Run_3D_Model_Painter.bat
echo if exist EnhancedFunModelPainter.exe ( >> Run_3D_Model_Painter.bat
echo     echo Starting Enhanced Fun Model Painter... >> Run_3D_Model_Painter.bat
echo     EnhancedFunModelPainter.exe >> Run_3D_Model_Painter.bat
echo ) else if exist FunModelPainter.exe ( >> Run_3D_Model_Painter.bat
echo     echo Starting Fun Model Painter... >> Run_3D_Model_Painter.bat
echo     FunModelPainter.exe >> Run_3D_Model_Painter.bat
echo ) else ( >> Run_3D_Model_Painter.bat
echo     echo No 3D Model Painter version was found! >> Run_3D_Model_Painter.bat
echo     echo Please run install.bat first. >> Run_3D_Model_Painter.bat
echo     pause >> Run_3D_Model_Painter.bat
echo ) >> Run_3D_Model_Painter.bat

echo + Created launcher: Run_3D_Model_Painter.bat

echo.
echo === INSTALLATION COMPLETE! ===
echo To start the application, double-click Run_3D_Model_Painter.bat
echo.
echo Enjoy painting your 3D models!
pause