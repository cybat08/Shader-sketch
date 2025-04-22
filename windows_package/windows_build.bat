@echo off
title 3D Model Painter Build Script
echo ================================================
echo   Building 3D Model Painter for Windows...
echo ================================================

setlocal
set "CPP_FILE=EnhancedModelPainter.cpp"
set "OUTPUT=3DModelPainter.exe"

rem Optional: make relative to script location
cd /d "%~dp0"

rem Check if Visual Studio compiler (cl.exe) is available
where cl >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [✓] Found Visual Studio compiler.
    
    if exist "resources.rc" (
        echo [*] Compiling resources with Visual Studio rc.exe...
        rc resources.rc
    )

    if exist "resources.res" (
        cl /EHsc /std:c++17 /Fe:"%OUTPUT%" "%CPP_FILE%" resources.res
    ) else (
        cl /EHsc /std:c++17 /Fe:"%OUTPUT%" "%CPP_FILE%"
    )

    if %ERRORLEVEL% NEQ 0 (
        echo [!] Build failed with Visual Studio compiler.
        goto try_mingw
    )
    echo [✓] Build successful using Visual Studio compiler!
    goto success
)

:try_mingw
rem Check if MinGW compiler is available
where g++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo [✓] Found MinGW compiler.
    
    where windres >nul 2>&1
    set "HAS_WINDRES=%ERRORLEVEL%"

    if exist "resources.rc" (
        if %HAS_WINDRES% EQU 0 (
            echo [*] Compiling resources with MinGW windres...
            windres resources.rc -O coff -o resources.res
        ) else (
            echo [!] windres not found, skipping icon embedding.
        )
    )

    if exist "resources.res" (
        g++ -std=c++17 "%CPP_FILE%" resources.res -o "%OUTPUT%" -static
    ) else (
        g++ -std=c++17 "%CPP_FILE%" -o "%OUTPUT%" -static
    )

    if %ERRORLEVEL% NEQ 0 (
        echo [!] Build failed with MinGW compiler.
        goto build_failed
    )
    echo [✓] Build successful using MinGW compiler!
    goto success
)

echo [✗] No C++ compiler found.
echo Install one of the following:
echo - Visual Studio with C++ Desktop Development
echo - MinGW-w64 with g++
goto end

:build_failed
echo ================================================
echo [✗] Build failed. Please check the errors above.
echo ================================================
goto end

:success
echo ================================================
echo [✓] Build completed successfully!
echo The executable is: "%OUTPUT%"
echo ================================================
echo.
echo Run the application with:
echo - Double-clicking "%OUTPUT%"
echo - Command line: "%OUTPUT%"
echo - Batch files:
echo    run_3d_model_painter.bat
echo    run_3d_model_painter_demo.bat
echo.

:end
pause
endlocal
