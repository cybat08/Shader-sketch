@echo off
echo ================================================
echo Building 3D Model Painter for Windows...
echo ================================================

set CPP_FILE=EnhancedModelPainter.cpp
set OUTPUT=3DModelPainter.exe

rem Check if Visual Studio compiler (cl.exe) is available
where cl >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Found Visual Studio compiler, using it for build...
    cl /EHsc /std:c++17 /Fe:%OUTPUT% %CPP_FILE%
    if %ERRORLEVEL% NEQ 0 (
        echo Build failed with Visual Studio compiler.
        goto try_mingw
    )
    echo Build successful using Visual Studio compiler!
    goto success
)

:try_mingw
rem Check if MinGW compiler (g++) is available
where g++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Found MinGW compiler, using it for build...
    g++ -std=c++17 %CPP_FILE% -o %OUTPUT% -static
    if %ERRORLEVEL% NEQ 0 (
        echo Build failed with MinGW compiler.
        goto build_failed
    )
    echo Build successful using MinGW compiler!
    goto success
)

echo No C++ compiler (Visual Studio or MinGW) found.
echo Please install either:
echo - Visual Studio with C++ Desktop Development workload
echo - MinGW-w64 with g++ compiler
goto end

:build_failed
echo ================================================
echo Build failed. Please check the error messages above.
echo ================================================
goto end

:success
echo ================================================
echo Build completed successfully!
echo The executable is available at: %OUTPUT%
echo ================================================
echo.
echo You can run the application by:
echo 1. Double-clicking on 3DModelPainter.exe
echo 2. Running "3DModelPainter.exe" from the command line
echo 3. Using the provided batch files:
echo    - run_3d_model_painter.bat (normal mode)
echo    - run_3d_model_painter_demo.bat (demo mode)
echo.

:end
pause