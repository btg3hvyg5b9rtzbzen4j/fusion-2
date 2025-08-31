#pragma once

#include "include/java/jni.h"
#include "include/java/jvmti.h"

extern JNIEnv *jenv;
extern JavaVM *jvm;
extern jvmtiEnv *tiEnv;

int setup_jenv();