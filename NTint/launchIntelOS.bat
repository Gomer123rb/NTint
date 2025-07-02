@echo off
title intelOS :: JOGL + LWJGL Launcher
setlocal

:: ── Paths & Sources ────────────────────────────────────────────────────────────
set LIB=lib
set SRC=intelBoot.java

:: ── Classpath: JOGL core + natives & LWJGL core + Vulkan + extras ─────────────
set CP=%LIB%\gluegen-rt.jar;^
%LIB%\gluegen-rt-natives-windows-amd64.jar;^
%LIB%\jogl-all.jar;^
%LIB%\jogl-all-natives-windows-amd64.jar;^
%LIB%\lwjgl.jar;^
%LIB%\lwjgl-vulkan.jar;^
%LIB%\lwjgl-glfw.jar;^
%LIB%\lwjgl-stb.jar

:: ── Audit lib Folder ──────────────────────────────────────────────────────────
echo [audit] Contents of %LIB%:
dir /b "%LIB%"
echo.

:: ── Build intelBoot.java ─────────────────────────────────────────────────────
echo [build] Compiling %SRC%...
javac -cp "%CP%" "%SRC%"
if errorlevel 1 (
    echo [build] Compile failed!
    pause
    exit /b
)

:: ── Launch intelOS Shell ──────────────────────────────────────────────────────
echo [launch] Starting intelOS shell...
java -Djava.library.path="%LIB%" -cp "%CP%;." intelBoot

endlocal
pause



