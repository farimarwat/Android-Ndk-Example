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