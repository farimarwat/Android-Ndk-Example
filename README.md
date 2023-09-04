# Android Ndk Example
How to include c++ library in android. Here is the quick and super simple example of "How you can add any c++ library in your android project and use its functions".

**Note:This tutorial is based on the project provided in the repo. To use the whole working project just clone it.**
### Requirements for this tutorial
1. You are already familiar with NDK
2. You are familiar with creating and including modules in app
   
So here in my case:
1. There is a simple library called "MyCalculator" which initialy contains two files. 

   a. Calculator.cpp
   
     b. Calculator.h
### Functionality of the library:
This library has super simple functions to calculate sum, multiply and subtraction

Calculator.cpp
```
int sum(int a, int b) {
	return a + b;
}
int sub(int a, int b) {
	return a - b;
}
int mul(int a, int b) {
	return a * b;
}
```

Calculator.h
```
#pragma once

int sum(int a, int b);
int sub(int a, int b);
int mul(int a, int b);
```
### Note: Every and each c++ library must have header (*.h) file(s). Without this you will not be able to include it. This header file helps the c++ compiler to compile the code and also used in JNI. While writing our code in JNI, we will use this header file to access all functions in JNI.

```
├── MyCalculator/
│ ├── Calculator.cpp
│ ├── Calculator.h
```
So now what we want is to include it in android and use its function via JNI. It is necessary to prepare the library prior to include it in android.

### Step 01 (Prepare the library):
Create a file called CmakeLists.txt inside the main folder of c++ library. In my case it is MyCalculator.
Now the folder structure will be like this:

```
├── MyCalculator/
│ ├── Calculator.cpp
│ ├── Calculator.h
│ ├── CMakeLists.txt
```

**CMakeLists.txt** is used by android NDK to compile the code. Without this your code will not be compiled. 
The main purpose of this file is to create a seperate lilbrary of c++ and save a file (*.so) in libs folder. You does not need to know underlaying logic. Just learn only CMake scripting.

CMakeLists.txt
```
# This is to define cmake version
cmake_minimum_required(VERSION 3.22.1)

# This means that we want to include all c++ (*.cpp) files in our library
file(GLOB_RECURSE CALCULATOR_SOURCES "*.cpp")

# This will create (*.so) file
add_library(
        MyCalculator             #Name it as you want. It is not necessary to keep like a parent folder name. So your resulting file will be = libMyCalculator.so
        SHARED                   #This means that you are telling NDK to create .so file
        ${CALCULATOR_SOURCES}    #Here you are providing the files which we declared above in file function
)

```
Thats it. You are done and your library is ready to use in android

### Step 02 (Create Native Module)

Now we need to create android new module which will be "Android Native Library" which is again super simple.
Do the following
```
File > New > New Module and here select "Android Native Library"
```
This will create a library module and will include it in your app. All files along with JNI interface will be create automatically.
If the create module is not included then do this:
```
File > Project Structure and next in panel select "Dependencies". Next selct "app" in left column, Press "+" in right column > Module Dependency. Here select your newly created module
```

Here in my case the new module name is "nativelib"

### Step 03 (Include "MyCalculator" Library):
Go to settings.gradle and add this:
```
include(":MyCalculator")
```
The resulting complete settings.gradle will be like this:
```
pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = "CalculatorExample"
include(":app")
include(":nativelib")
include(":MyCalculator")

```

Now navigate to your nativelib module and findout another CMakeLists.txt which is main cmake file and will be similar to the one which we have created before for MyCalculator.
The newly created module "nativelib" folder structure will be like this:

```
├── nativelib/
│ ├── manifests
│ ├── java
│ ├── cpp
| │ ├── nativelib.cpp
| │ ├── CMakeLists.txt
```

Now make these changes to this main CMakeLists.txt. Here we tell to NDK via this main CMakeLists.txt that include our MyCalculator .so file which will created by NDK

Add these:
```
#This tells the NDK to add header file. So we can access all functiions in JNI
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../MyCalculator)
#This tells the NDK to include our library. The name should be same as we used before in add_library() function
add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../MyCalculator
        MyCalculator
)
```

So the overall resulting main CMakeLists.txt file will be:
```

# Sets the minimum CMake version required for this project.
cmake_minimum_required(VERSION 3.22.1)

project("nativelib")

#This tells the NDK to add header file. So we can access all functiions in JNI
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../../MyCalculator)

#This tells the NDK to include our library. The name should be same as we used before in add_library() function
add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../MyCalculator
        MyCalculator
)
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        nativelib.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
        MyCalculator
        android
        log)
```

**Congratulations. You are done**

### Final Step (Use functions)

1. Now what i want is to use MyCalculator library to sum two integers. Open nativelib.cpp which is automaticall created while creating our module.
   Include "Calculator.h" file to use sum() function.
2. Next use the function. It will will automaticall catch by android stuidon inteli sense. If not caught then you are missing something.

nativelib.cpp
```
#include <jni.h>
#include <string>
#include "Calculator.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_fungiggle_nativelib_NativeLib_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    int result = sum(20,41);
    std::string hello = "Sum of 20 and 41 is: ";
    hello += std::to_string(result);
    return env->NewStringUTF(hello.c_str());
}
```
Done. Now use the function stringFromJNI() any where in java/kotlin
