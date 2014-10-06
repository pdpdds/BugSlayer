if not exist .\WinDebug/NUL mkdir .\WinDebug
if not exist ..\..\..\Output/NUL mkdir ..\..\..\Output
mc.exe  -s -c  AssertTest.mc
rc.exe  -D_X86_=1  /l 0x409    -I..\..\..\include /d "_DEBUG" /d "DEBUG" /fo.\WinDebug\AssertTest.res AssertTest.rc
cl.exe  -D_X86_=1 /DWIN32 /D_WINDOWS /DSTRICT /D_DEBUG /DDEBUG /D_MBCS /D_AFXDLL  /nologo /WX /GX    /MDd /Od /W3 /c  /Zi        /GZ                /Fp.\WinDebug\DiagAssert.pch -I..\..\..\include /Fd..\..\..\Output/"DiagAssert.pdb" /YX.\WinDebug\DiagAssert.pch /Fo.\WinDebug/ AssertTest.cpp
link.exe kernel32.lib user32.lib gdi32.lib advapi32.lib ..\..\..\Output\BugslayerUtil.lib    /NOLOGO /MACHINE:i386 /OUT:..\..\..\Output/DiagAssert.exe   /SUBSYSTEM:console /INCREMENTAL:yes  /DEBUG /PDB:..\..\..\Output/"DiagAssert.pdb" /PDBTYPE:CON   .\WinDebug/AssertTest.OBJ  .\WinDebug/AssertTest.RES
