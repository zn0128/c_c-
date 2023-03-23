#include <stdio.h>

#include "HelloNative.h"

JNIEXPORT void JNICALL Java_HelloNative_sayHello(JNIEnv *env, jclass jc)//难点二

{

    printf("Hello Native\n");

}