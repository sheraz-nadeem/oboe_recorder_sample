#include <jni.h>
#include <string>
#include <oboe/oboe.h>

extern "C" JNIEXPORT jstring JNICALL
Java_com_sheraz_oboerecorder_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
