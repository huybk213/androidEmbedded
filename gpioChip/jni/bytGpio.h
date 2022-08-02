//
// Created by HuyTV on 7/28/2022.
//

#ifndef MY_APPLICATION_BYTGPIO_H
#define MY_APPLICATION_BYTGPIO_H

#include "jni.h"
#include "gpiod.h"

jint Java_com_example_gpio_Gpio_setGpioStatus
	(JNIEnv *env, jobject clazz, jlong gpioObj, jint value);
jint Java_com_example_gpio_Gpio_setOutputMode
	(JNIEnv *env, jobject clazz, jlong gpioObj);

jlong Java_com_example_gpio_Gpio_createIO
	(JNIEnv *env, jobject clazz, jint gpioId);

void Java_com_example_gpio_Gpio_deleteIO
	(JNIEnv *env, jobject clazz, jlong ptr);

class bytGpioApi
{
public:
    bytGpioApi();
    ~bytGpioApi();
    int release();
    int gpioSetModeOutput();
    int gpioWriteOutput(int value);
    int gpioReadOutput();
    int gpioSetInput();
    int gpioReadInput();
    int begin(int id);
private:
    const char *chipname;
    int pin;
    struct gpiod_chip *chip;
    struct gpiod_line *lineIO;
    char name[128];
    int status;
};

#endif //MY_APPLICATION_BYTGPIO_H
