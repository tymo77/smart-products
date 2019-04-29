# Smart Products ME 5194 Spring 2019

This repository contains all my code for Dr. Metzler's Spring 2019 Smart Products Design course. This course is primarly focused on use of the Rasperry Pi 3 and C++.

## Getting Started

The course consists of a series of labs. The code needed for each lab can be found in the [labs](labs/) subdirectories by number. Each lab has a subdirectory for builds on both Windows [(eg)](labs/1/win/) and Linux [(eg)](labs/2/lin/) with nmake and GNU make makefiles accordingly. Instructions and references for the lab are also contained in the /instructions subdirectories [(eg)](labs/1/instructions/).

To build a lab on Windows, run 

```
nmake
```

from the lab win directory.


To build a lab on Linux, run 

```
make
```

from the lab lin directory.

## File Structure

```
.
+-- include/
|   +-- SPmath.h
|   +-- SPmatrix.h
|   :
|   :
|   :
|   +-- *.h
+-- source/
|   +-- SPmath.cpp
|   +-- SPmatrix.cpp
|   :
|   :
|   :
|   +-- *.cpp
+-- labs/
|   +-- 1/
|       +-- labone.cpp/
|       +-- instructions/
|       +-- lin/
|           +-- makefile
|       +-- win/
|           +-- makefile
|   +-- 2/
|       +-- labtwo.cpp/
|       +-- instructions/
|       +-- lin/
|           +-- makefile
|       +-- win/
|           +-- makefile
|       :
|       :
|       :
|   +-- n/
|       +-- main.cpp/
|       +-- instructions/
|       +-- lin/
|           +-- makefile
|       +-- win/
|           +-- makefile
```

## Authors

* **Tyler Morrison** (morrison.730)

Labs 6-10, Project, Motor_Project:
* **Yuan Gao** (gao.1492)
