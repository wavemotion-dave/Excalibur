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
	-@erase "$(INTDIR)\Ex_buis.obj"
	-@erase "$(INTDIR)\Ex_comp.obj"
	-@erase "$(INTDIR)\ex_complex.obj"
	-@erase "$(INTDIR)\Ex_CompSci.obj"
	-@erase "$(INTDIR)\Ex_conv.obj"
	-@erase "$(INTDIR)\Ex_cust.obj"
	-@erase "$(INTDIR)\Ex_geom.obj"
	-@erase "$(INTDIR)\Ex_misc.obj"
	-@erase "$(INTDIR)\ex_phys.obj"
	-@erase "$(INTDIR)\Ex_prog.obj"
	-@erase "$(INTDIR)\Ex_sci.obj"
	-@erase "$(INTDIR)\Ex_stat.obj"
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
	"$(INTDIR)\Ex_buis.obj" \
	"$(INTDIR)\Ex_comp.obj" \
	"$(INTDIR)\ex_complex.obj" \
	"$(INTDIR)\Ex_CompSci.obj" \
	"$(INTDIR)\Ex_conv.obj" \
	"$(INTDIR)\Ex_cust.obj" \
	"$(INTDIR)\Ex_geom.obj" \
	"$(INTDIR)\Ex_misc.obj" \
	"$(INTDIR)\ex_phys.obj" \
	"$(INTDIR)\Ex_prog.obj" \
	"$(INTDIR)\Ex_sci.obj" \
	"$(INTDIR)\Ex_stat.obj" \
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
	-@erase "$(INTDIR)\Ex_buis.obj"
	-@erase "$(INTDIR)\Ex_comp.obj"
	-@erase "$(INTDIR)\ex_complex.obj"
	-@erase "$(INTDIR)\Ex_CompSci.obj"
	-@erase "$(INTDIR)\Ex_conv.obj"
	-@erase "$(INTDIR)\Ex_cust.obj"
	-@erase "$(INTDIR)\Ex_geom.obj"
	-@erase "$(INTDIR)\Ex_misc.obj"
	-@erase "$(INTDIR)\ex_phys.obj"
	-@erase "$(INTDIR)\Ex_prog.obj"
	-@erase "$(INTDIR)\Ex_sci.obj"
	-@erase "$(INTDIR)\Ex_stat.obj"
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
	"$(INTDIR)\Ex_buis.obj" \
	"$(INTDIR)\Ex_comp.obj" \
	"$(INTDIR)\ex_complex.obj" \
	"$(INTDIR)\Ex_CompSci.obj" \
	"$(INTDIR)\Ex_conv.obj" \
	"$(INTDIR)\Ex_cust.obj" \
	"$(INTDIR)\Ex_geom.obj" \
	"$(INTDIR)\Ex_misc.obj" \
	"$(INTDIR)\ex_phys.obj" \
	"$(INTDIR)\Ex_prog.obj" \
	"$(INTDIR)\Ex_sci.obj" \
	"$(INTDIR)\Ex_stat.obj" \
	"$(INTDIR)\Excal.obj" \
	"$(INTDIR)\Excal.res"

"$(OUTDIR)\excal.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "excal - Win32 Release" || "$(CFG)" == "excal - Win32 Debug"
SOURCE=.\Ex_buis.c
DEP_CPP_EX_BU=\
	".\excal.h"\
	

"$(INTDIR)\Ex_buis.obj" : $(SOURCE) $(DEP_CPP_EX_BU) "$(INTDIR)"


SOURCE=.\Ex_comp.cpp
DEP_CPP_EX_CO=\
	".\excal.h"\
	

"$(INTDIR)\Ex_comp.obj" : $(SOURCE) $(DEP_CPP_EX_CO) "$(INTDIR)"


SOURCE=.\ex_complex.c
DEP_CPP_EX_COM=\
	".\excal.h"\
	

"$(INTDIR)\ex_complex.obj" : $(SOURCE) $(DEP_CPP_EX_COM) "$(INTDIR)"


SOURCE=.\Ex_CompSci.c
DEP_CPP_EX_COMP=\
	".\excal.h"\
	

"$(INTDIR)\Ex_CompSci.obj" : $(SOURCE) $(DEP_CPP_EX_COMP) "$(INTDIR)"


SOURCE=.\Ex_conv.c
DEP_CPP_EX_CON=\
	".\excal.h"\
	

"$(INTDIR)\Ex_conv.obj" : $(SOURCE) $(DEP_CPP_EX_CON) "$(INTDIR)"


SOURCE=.\Ex_cust.c
DEP_CPP_EX_CU=\
	".\excal.h"\
	

"$(INTDIR)\Ex_cust.obj" : $(SOURCE) $(DEP_CPP_EX_CU) "$(INTDIR)"


SOURCE=.\Ex_geom.c
DEP_CPP_EX_GE=\
	".\excal.h"\
	

"$(INTDIR)\Ex_geom.obj" : $(SOURCE) $(DEP_CPP_EX_GE) "$(INTDIR)"


SOURCE=.\Ex_misc.c
DEP_CPP_EX_MI=\
	".\excal.h"\
	

"$(INTDIR)\Ex_misc.obj" : $(SOURCE) $(DEP_CPP_EX_MI) "$(INTDIR)"


SOURCE=.\ex_phys.c
DEP_CPP_EX_PH=\
	".\excal.h"\
	

"$(INTDIR)\ex_phys.obj" : $(SOURCE) $(DEP_CPP_EX_PH) "$(INTDIR)"


SOURCE=.\Ex_prog.c
DEP_CPP_EX_PR=\
	".\excal.h"\
	

"$(INTDIR)\Ex_prog.obj" : $(SOURCE) $(DEP_CPP_EX_PR) "$(INTDIR)"


SOURCE=.\Ex_sci.c
DEP_CPP_EX_SC=\
	".\excal.h"\
	

"$(INTDIR)\Ex_sci.obj" : $(SOURCE) $(DEP_CPP_EX_SC) "$(INTDIR)"


SOURCE=.\Ex_stat.c
DEP_CPP_EX_ST=\
	".\excal.h"\
	

"$(INTDIR)\Ex_stat.obj" : $(SOURCE) $(DEP_CPP_EX_ST) "$(INTDIR)"


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

