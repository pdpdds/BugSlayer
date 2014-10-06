if not exist .\WinDebug/NUL mkdir .\WinDebug
if not exist ..\..\..\Output/NUL mkdir ..\..\..\Output
cl.exe -D_X86_=1 /DWIN32 /D_WINDOWS /DSTRICT /D_DEBUG /DDEBUG /D_MBCS /D_AFXDLL /nologo /WX /GX /MDd /Od /W3 /c /Z7 /Fp.\WinDebug\SymTest.pch /YX.\WinDebug\SymTest.pch /Fo.\WinDebug/ /I..\..\..\Include SymTest.cpp
echo ...Linking .\Output/SymTest.exe
link.exe kernel32.lib user32.lib gdi32.lib advapi32.lib DBGHELP.lib /NOLOGO /MACHINE:i386 /OUT:..\..\..\Output/SymTest_C7.exe /SUBSYSTEM:console /DEBUG:full /DEBUGTYPE:CV /INCREMENTAL:no /PDB:none .\WinDebug/SymTest.OBJ
