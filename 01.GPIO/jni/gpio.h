#ifndef __GPIO_H__
#define __GPIO_H__

#include <jni.h>
#include <android/log.h>

#define LOG_TAG "NativeGpioJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

// These are documented on the Java side, in NativeWebcam
jint java_com_example_gpio_Gpio_getGpioStatus(JNIEnv* env,
        jobject thiz, jint gpioId);
		
jint Jjava_com_example_gpio_Gpio_setGpioStatus(JNIEnv* env,
        jobject thiz, jint gpioId, jint gpioStatus);
		
jint Jjava_com_example_gpio_Gpio_getAdcStatus(JNIEnv* env,
        jobject thiz);

#endif // __GPIO_H__
