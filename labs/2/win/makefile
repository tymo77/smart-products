CPP = cl
CPP_LIST = ..\labtwo.cpp ..\..\..\source\SPmath.cpp ..\..\..\source\SPmatrix.cpp

build: main

main: $(CPP_LIST)
	$(CPP) /Fe..\bin\$(@B) $** /FC
    

clean:
    del *.obj ..\bin\main.exe