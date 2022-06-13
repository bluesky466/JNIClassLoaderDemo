#include <jni.h>
#include <string>
#include "android/log.h"
#include <pthread.h>

static const char *TAG = "JniDemo";
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)

JavaVM *javaVM;
jclass clazzMainActivity;

jclass findClass(JNIEnv *env, const char *className) {
    jclass clazz = env->FindClass(className);
    LOGD("%s : %ld", className, clazz);
    return clazz;
}

void *threadFunc(void *arg) {
    JNIEnv *env = NULL;
    bool needDetach = false;
    if (javaVM->GetEnv((void **) &env, JNI_VERSION_1_4) < 0) {
        LOGD("----AttachCurrentThread----");
        javaVM->AttachCurrentThread(&env, NULL);
        needDetach = true;
    }

    jfieldID field = env->GetStaticFieldID(clazzMainActivity, "DATA_IN_JAVA", "I");
    int data = env->GetStaticIntField(clazzMainActivity, field);
    LOGD("data in threadFunc : %d", data);

    findClass(env, "java/lang/String");
    findClass(env, "android/util/Log");
    findClass(env, "android/app/Activity");
    findClass(env, "me/linjw/demo/jni/MainActivity");
    env->ExceptionClear();

    if (needDetach) {
        LOGD("----DetachCurrentThread----");
        javaVM->DetachCurrentThread();
    }

    return NULL;
}


extern "C" JNIEXPORT void JNICALL
Java_me_linjw_demo_jni_MainActivity_test(
        JNIEnv *env,
        jobject /* this */) {
    jclass clazz = findClass(env, "me/linjw/demo/jni/MainActivity");
    jfieldID field = env->GetStaticFieldID(clazz, "DATA_IN_JAVA", "I");
    int data = env->GetStaticIntField(clazz, field);
    LOGD("data : %d", data);

    clazzMainActivity = (jclass) env->NewGlobalRef(clazz);

    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);
}


jint JNI_OnLoad(JavaVM *vm, void * /* reserved */) {
    javaVM = vm;
    return JNI_VERSION_1_4;
}