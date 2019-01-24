CPP = cl
src = ..\..\..\source\\

build: main

main: SPmatrix.obj SPmath.obj ..\labtwo.cpp
	@echo Building lab file...
	$(CPP) /Fe$@ $** /FC /EHsc /nologo
	
SPmatrix.obj: $(src)SPmatrix.cpp
	@echo Building SPmatrix...
	$(CPP) /c $** /EHsc /nologo
	
SPmath.obj: $(src)SPmath.cpp
	@echo Building SPmath...
	$(CPP) /c $** /EHsc /nologo
    
clean:
	@echo Cleaning...
    del *.obj main.exe