@echo OFF

IF /I "%1"=="uninstall" (
echo Stopping old service version...
net stop "CppFastCGI"
echo Uninstalling old service version...
sc delete "CppFastCGI"
)

IF /I "%1"=="install" (
echo Installing service...
rem DO NOT remove the space after "binpath="!
sc create "CppFastCGI" binpath= "%cd%\Debug\cppfastcgi-daemon.exe" start= auto
echo Starting server complete
)

IF /I "%1"=="start" (
echo Starting service...
net start "CppFastCGI"
)

IF /I "%1"=="stop" (
echo Stopping service...
net stop "CppFastCGI"
)

pause
