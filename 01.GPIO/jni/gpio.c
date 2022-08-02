#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
// #include <linux/gpio.h>
#include "gpiod.h"

//
// Created by HuyTV on 7/28/2022.
//

#define DEVICE_NAME_ADC	"/sys/class/power_supply/battery/mpp4_voltage"
#define ERROR  -1

// static void throw_NullPointerException(JNIEnv *env, const char* msg)
// {
    // jclass clazz;
    // clazz = env->FindClass("java/lang/NullPointerException");
    // env->ThrowNew(clazz, msg);
// }

struct gpiod_chip *chip;
struct gpiod_line *lineIO;
const char *chipname = "gpiochip0";

static int openGpio(jint gpioId, jint mode)
{
    const char *chipname = "gpiochip0";
    // Orangepi3 gpiochip0 (0-64)
    if (gpioId >= 64 && gpioId <= 256)
    {
        chipname = "gpiochip1";
    }
    // else if (gpioId > 256)
    //   {
    //        chipname = "gpiochip2";
    //  }

    int ret = -1, val;

    // Open GPIO chip
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
		LOGE("Open chip %s io %d failed, error %d, mean %s\r\n", chipname, gpioId, errno, strerror(errno));
		goto end;
	}

    // LOGI("Max number of line %d\r\n", gpiod_chip_num_lines(chip));
    // Open GPIO lines
    lineIO = gpiod_chip_get_line(chip, gpioId);
    if (!lineIO) {
		LOGE("[%s] Get line on pin %d failed, error %d - %s\r\n", __FUNCTION__, gpioId, errno, strerror(errno));
		goto close_chip;
	}

    if (mode == 0) {
        // Open IO lines for output
        ret = gpiod_line_request_output(lineIO, "GPIO_JNI", 0);
        if (ret < 0) {
            LOGE("Request line %d as output failed %d\r\n", gpioId, errno);
            goto release_line;
        }
        else {
            gpiod_line_set_value(lineIO, 0);
        }
    }
    else if (mode == 0) {
        // Open IO lines for input
        ret = gpiod_line_request_input(lineIO, "GPIO_JNI");
        if (ret < 0) {
            LOGE("Request line %d as input failed %d\r\n", gpioId, errno);
            goto release_line;
        }
    }

    ret = 0;
    return 0;

release_line:
    // Release lines and chip
    gpiod_line_release(lineIO);
close_chip:
    gpiod_chip_close(chip);
end:
    return ret;
}

static int closeGpio(int gpioId) {
    int err = 0;
    gpiod_line_release(lineIO);
    gpiod_chip_close(chip);
    return 0;
}

static int setMode(jint gpioId, jint mode)
{
    int ret = -1;
    if (mode == 0) {
        ret = gpiod_line_request_output(lineIO, "GPIO_JNI", 0);
    } 
    else {
        ret = gpiod_line_request_input(lineIO, "GPIO_JNI");
    }

    return ret;
}

/*
* write GPIO
* params: gpioId, newValue
* return: 0: success, otherwise failed
*/
static int setGpioStatus(jint gpioId, jint gpioStatus)
{
    int ret = -1, val;

    ret = gpiod_line_set_value(lineIO, gpioStatus);
    if (ret == -1)
    {
        LOGI("Set gpio failed, error %d - %s\r\n", errno, strerror(errno));
    }
    return ret;
}

/*
* read GPIO
* params: gpioId
* return: gpioState: ok, < 0 failed
*/
static int getGpioStatus(jint gpioId)
{
    int val;
    val = gpiod_line_get_value(lineIO);
    if (val == -1)
    {
        LOGE("GPIO get line value, error no %d, mean %s\r\n", errno, strerror(errno));
    }
    else
    {
        LOGD("GPIO%d = %d\r\n", gpioId, val);
    }
    return val;
}

jint Java_com_example_gpio_Gpio_openGpio
	(JNIEnv *env, jobject clazz, jint gpioId, jint mode)
{
	//LOGI("getGpioStatus %d", gpioId);
	int ret = openGpio(gpioId, mode);
	return ret;
}

jint Java_com_example_gpio_Gpio_closeGpio
	(JNIEnv *env, jobject clazz, jint gpioId)
{
	//LOGI("getGpioStatus %d", gpioId);
	int ret = closeGpio(gpioId);
	return ret;
}

jint Java_com_example_gpio_Gpio_getGpioStatus
	(JNIEnv *env, jobject clazz, jint gpioId)
{
	//LOGI("getGpioStatus %d", gpioId);
	int ret = getGpioStatus(gpioId);
	return ret;
}

/*
* Java_com_binhanh_periph_gpio_Gpio_setGpioStatus
* params: (I)gpioId, (I)gpioStatus
* return: (I)
*/
jint Java_com_example_gpio_Gpio_setGpioStatus
	(JNIEnv *env, jobject clazz, jint gpioId, jint gpioStatus)
{
	int ret = setGpioStatus(gpioId, gpioStatus);
	return ret;
}

/*
* Java_com_binhanh_periph_gpio_Gpio_setGpioStatus
* params: (I)gpioId, (I)gpioStatus
* return: (I)
*/
jint Java_com_example_gpio_Gpio_setMode
	(JNIEnv *env, jobject clazz, jint gpioId, jint mode)
{
	int ret = setMode(gpioId, mode);
	return ret;
}

