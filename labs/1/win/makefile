CPP = cl
#OBJ_LIST = ..\labone.obj ..\..\..\source\SPmath.obj
CPP_LIST = ..\labone.cpp ..\..\..\source\SPmath.cpp

build: main

main: $(CPP_LIST)
	$(CPP) /Fe..\bin\$(@B) $** /FC
    

clean:
    del *.obj ..\bin\main.exe