@echo OFF
echo Stopping old service version...
net stop "CppFastCGI"
echo Uninstalling old service version...
sc delete "CppFastCGI"

echo Installing service...
rem DO NOT remove the space after "binpath="!
sc create "CppFastCGI" binpath= "Debug\cppfastcgi-daemon.exe" start= auto
echo Starting server complete
pause
