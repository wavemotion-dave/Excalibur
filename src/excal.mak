# Microsoft Developer Studio Generated NMAKE File, Based on excal.dsp
!IF "$(CFG)" == ""
CFG=excal - Win32 Release
!MESSAGE No configuration specified. Defaulting to excal - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "excal - Win32 Release" && "$(CFG)" != "excal - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "excal.mak" CFG="excal - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "excal - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "excal - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "excal - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Excal32.exe"

!ELSE 

ALL : "$(OUTDIR)\Excal32.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Financial.obj"
	-@erase "$(INTDIR)\ComplexMath.obj"
	-@erase "$(INTDIR)\CompSci.obj"
	-@erase "$(INTDIR)\Conversion.obj"
	-@erase "$(INTDIR)\Custom.obj"
	-@erase "$(INTDIR)\Constants.obj"
	-@erase "$(INTDIR)\Programming.obj"
	-@erase "$(INTDIR)\Scientific.obj"
	-@erase "$(INTDIR)\Statistics.obj"
	-@erase "$(INTDIR)\Excal.obj"
	-@erase "$(INTDIR)\Excal.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\Excal32.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\excal.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Excal.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\excal.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\Excal32.pdb" /machine:I386 /out:"$(OUTDIR)\Excal32.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Financial.obj" \
	"$(INTDIR)\ComplexMath.obj" \
	"$(INTDIR)\CompSci.obj" \
	"$(INTDIR)\Conversion.obj" \
	"$(INTDIR)\Custom.obj" \
	"$(INTDIR)\Constants.obj" \
	"$(INTDIR)\Programming.obj" \
	"$(INTDIR)\Scientific.obj" \
	"$(INTDIR)\Statistics.obj" \
	"$(INTDIR)\Excal.obj" \
	"$(INTDIR)\Excal.res"

"$(OUTDIR)\Excal32.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "excal - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\excal.exe"

!ELSE 

ALL : "$(OUTDIR)\excal.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Financial.obj"
	-@erase "$(INTDIR)\ComplexMath.obj"
	-@erase "$(INTDIR)\CompSci.obj"
	-@erase "$(INTDIR)\Conversion.obj"
	-@erase "$(INTDIR)\Custom.obj"
	-@erase "$(INTDIR)\Programming.obj"
	-@erase "$(INTDIR)\Scientific.obj"
	-@erase "$(INTDIR)\Statistics.obj"
	-@erase "$(INTDIR)\Excal.obj"
	-@erase "$(INTDIR)\Excal.res"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\excal.exe"
	-@erase "$(OUTDIR)\excal.ilk"
	-@erase "$(OUTDIR)\excal.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\excal.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Excal.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\excal.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\excal.pdb" /debug /machine:I386 /out:"$(OUTDIR)\excal.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Financial.obj" \
	"$(INTDIR)\ComplexMath.obj" \
	"$(INTDIR)\CompSci.obj" \
	"$(INTDIR)\Conversion.obj" \
	"$(INTDIR)\Custom.obj" \
	"$(INTDIR)\Constants.obj" \
	"$(INTDIR)\Programming.obj" \
	"$(INTDIR)\Scientific.obj" \
	"$(INTDIR)\Ex_stat.obj" \
	"$(INTDIR)\Excal.obj" \
	"$(INTDIR)\Excal.res"

"$(OUTDIR)\excal.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "excal - Win32 Release" || "$(CFG)" == "excal - Win32 Debug"
SOURCE=.\Financial.c
DEP_CPP_EX_BU=\
	".\excal.h"\
	

"$(INTDIR)\Financial.obj" : $(SOURCE) $(DEP_CPP_EX_BU) "$(INTDIR)"


SOURCE=.\ComplexMath.cpp
DEP_CPP_EX_CO=\
	".\excal.h"\
	

"$(INTDIR)\ComplexMath.obj" : $(SOURCE) $(DEP_CPP_EX_CO) "$(INTDIR)"


SOURCE=.\CompSci.c
DEP_CPP_EX_COMP=\
	".\excal.h"\
	

"$(INTDIR)\CompSci.obj" : $(SOURCE) $(DEP_CPP_EX_COMP) "$(INTDIR)"


SOURCE=.\Conversion.c
DEP_CPP_EX_CON=\
	".\excal.h"\
	

"$(INTDIR)\Conversion.obj" : $(SOURCE) $(DEP_CPP_EX_CON) "$(INTDIR)"


SOURCE=.\Custom.c
DEP_CPP_EX_CU=\
	".\excal.h"\
	

"$(INTDIR)\Custom.obj" : $(SOURCE) $(DEP_CPP_EX_CU) "$(INTDIR)"


SOURCE=.\Constants.c
DEP_CPP_EX_CO=\
	".\excal.h"\
	

"$(INTDIR)\Constants.obj" : $(SOURCE) $(DEP_CPP_EX_CO) "$(INTDIR)"


SOURCE=.\Programming.c
DEP_CPP_EX_PR=\
	".\excal.h"\
	

"$(INTDIR)\Programming.obj" : $(SOURCE) $(DEP_CPP_EX_PR) "$(INTDIR)"


SOURCE=.\Scientific.c
DEP_CPP_EX_SC=\
	".\excal.h"\
	

"$(INTDIR)\Scientific.obj" : $(SOURCE) $(DEP_CPP_EX_SC) "$(INTDIR)"


SOURCE=.\Statistics.c
DEP_CPP_EX_ST=\
	".\excal.h"\
	

"$(INTDIR)\Statistics.obj" : $(SOURCE) $(DEP_CPP_EX_ST) "$(INTDIR)"


SOURCE=.\Excal.c
DEP_CPP_EXCAL=\
	".\excal.h"\


"$(INTDIR)\Excal.obj" : $(SOURCE) $(DEP_CPP_EXCAL) "$(INTDIR)"


SOURCE=.\Excal.rc
DEP_RSC_EXCAL_=\
	".\excal.h"\
	".\excal.ico"\
	

"$(INTDIR)\Excal.res" : $(SOURCE) $(DEP_RSC_EXCAL_) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

