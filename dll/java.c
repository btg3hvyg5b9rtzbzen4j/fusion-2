#include "java.h"

JNIEnv *jenv = NULL;
JavaVM *jvm = NULL;
jvmtiEnv *tiEnv = NULL;

int setup_jenv() {
  jenv = NULL;
  jvm = NULL;
  jsize count = 0;

  // this method is retardedly misleading since a process can only have 1 java vm at a time 😁
  if (JNI_GetCreatedJavaVMs(&jvm, 1, &count) != JNI_OK || count == 0)
    return 0;

  (*jvm)->GetEnv(jvm, (void **)&jenv, JNI_VERSION_1_6);
  (*jvm)->GetEnv(jvm, (void **)&tiEnv, JVMTI_VERSION);

  // in the old code we did a check for JNI_EDETACHED, but that was useless since this thread wont be attached to the jvm anyways
  (*jvm)->AttachCurrentThread(jvm, (void **)&jenv, NULL);
}