CPP = cl
src = ..\..\..\source\\

build: main

main: SPmath.obj ..\labone.cpp
	@echo Building lab file...
	$(CPP) /Fe$@ $** /FC /EHsc /nologo
	
SPmath.obj: $(src)SPmath.cpp
	@echo Building SPmath...
	$(CPP) /c $** /EHsc /nologo
    
clean:
	@echo Cleaning...
    del *.obj main.exe