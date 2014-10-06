if not exist .\WinDebug/NUL mkdir .\WinDebug
if not exist ..\..\..\Output/NUL mkdir ..\..\..\Output
cl.exe  -D_X86_=1 /DWIN32 /D_WINDOWS /DSTRICT /D_DEBUG /DDEBUG /D_MBCS /D_AFXDLL  /nologo /WX /GX    /MDd /Od /W3 /c  /Z7          /Fp.\WinDebug\CH_Tests.pch -I..\..\..\include /YX.\WinDebug\CH_Tests.pch /Fo.\WinDebug/ CH_Tests.cpp
link.exe kernel32.lib user32.lib gdi32.lib advapi32.lib ..\..\..\output\BugslayerUtil.lib DBGHELP.LIB    /NOLOGO /MACHINE:i386 /OUT:..\..\..\Output/CH_Tests_C7.exe   /SUBSYSTEM:console /DEBUG:full /DEBUGTYPE:both  /INCREMENTAL:no /PDB:none   .\WinDebug/CH_Tests.OBJ

