#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
// #include <linux/gpio.h>
#include "gpiod.h"
#include "jni.h"
#include <dirent.h>

//
// Created by HuyTV on 7/28/2022.
//

typedef struct
{
    struct gpiod_chip *chip;
    struct gpiod_line *lineIO;
    const char *chipname;
    int id;
} gpioInfo_t;

static gpioInfo_t *mMaxnumberOfChipInfo = (gpioInfo_t*)0;
static int numChips = 0;

static int dir_filter(const struct dirent *dir)
{
	return !strncmp(dir->d_name, "gpiochip", 8);
}

static long openGpio(jint gpioId, jint mode)
{
    bool insertNewChip = true;
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
    if (!mMaxnumberOfChipInfo) {
        struct dirent **dir;
        numChips = scandir("/dev", &dir, dir_filter, alphasort);
        if (numChips < 0) {
            LOGE("No gpiochip found\r\n");
            return 0;
        }
        LOGI("Number of chip %d\r\n", numChips);
        mMaxnumberOfChipInfo = malloc(numChips * sizeof(gpioInfo_t));
        if (mMaxnumberOfChipInfo) {
            LOGE("Malloc chip memory\r\n");
            memset(mMaxnumberOfChipInfo, 0, numChips*sizeof(gpioInfo_t));
        } else {
            LOGE("Malloc %d gpiochips failed\r\n", numChips);
            return 0;
        }
    }

    if (!mMaxnumberOfChipInfo)
    {
        LOGE("null chip info\r\n");
        return 0;
    }

    LOGE("Alloc gpio %d info\r\n", gpioId);
    gpioInfo_t *dev = malloc(sizeof(gpioInfo_t));
    if (!dev) {
        return 0;
    }
    dev->id = gpioId;
    // assert(dev);
    // Open GPIO chip
    dev->chipname = chipname;
    dev->chip = (void*)0;

    // check if chip is opened
    int first_free_index = -1;
    for (int i = 0; i < numChips; i++) {
        if (mMaxnumberOfChipInfo[i].chipname
            && strlen(mMaxnumberOfChipInfo[i].chipname)
            && (strcmp(chipname, mMaxnumberOfChipInfo[i].chipname) == 0)) {
                LOGI("Chip %s already opened", chipname);
                dev->chip = mMaxnumberOfChipInfo[i].chip;
                insertNewChip = false;
                break;
        }
    }
    if (!dev->chip) {
        LOGI("Open new chip");
        dev->chip = gpiod_chip_open_by_name(chipname);
    }
    if (!dev->chip) {
		LOGE("Open chip %s io %d failed, error %d, mean %s\r\n", chipname, gpioId, errno, strerror(errno));
		goto end;
	}

    // LOGI("Max number of line %d\r\n", gpiod_chip_num_lines(chip));
    // Open GPIO lines
    dev->lineIO = gpiod_chip_get_line(dev->chip, gpioId);
    if (!dev->lineIO) {
		LOGE("[%s] Get line on pin %d failed, error %d - %s\r\n", __FUNCTION__, gpioId, errno, strerror(errno));
		goto close_chip;
	}

    if (mode == 0) {
        LOGI("Mode output\r\n");
        // Open IO lines for output
        ret = gpiod_line_request_output(dev->lineIO, "Out", 0);
        if (ret < 0) {
            LOGE("Request line %d as output failed %d\r\n", gpioId, errno);
            goto release_line;
        }
        else {
            gpiod_line_set_value(dev->lineIO, 0);
        }
    }
    else if (mode == 1) {
        // Open IO lines for input
        LOGI("Mode input\r\n");
        ret = gpiod_line_request_input(dev->lineIO, "In");
        if (ret < 0) {
            LOGE("Request line %d as input failed %d\r\n", gpioId, errno);
            goto release_line;
        }
    }

    for (int i = 0; i < numChips; i++) {
        if (mMaxnumberOfChipInfo[i].lineIO == 0) {
            LOGI("Insert new chipname [%u] %s", i , chipname);
            memcpy(&mMaxnumberOfChipInfo[i], dev, sizeof(gpioInfo_t));
            break;
        }
    }

    return (long)dev;

release_line:
    // Release lines and chip
    gpiod_line_release(dev->lineIO);
close_chip:
    gpiod_chip_close(dev->chip);
    free(dev);
mem:
end:
    return 0;
}

static int closeGpio(gpioInfo_t *info) {
    int err = 0;
    gpiod_line_release(info->lineIO);
    gpiod_chip_close(info->chip);
    free(info);
    return 0;
}

static int setMode(gpioInfo_t *info, jint mode)
{
    int ret = -1;
    if (mode == 0) {
        ret = gpiod_line_request_output(info->lineIO, "GPIO_JNI", 0);
    } 
    else {
        ret = gpiod_line_request_input(info->lineIO, "GPIO_JNI");
    }
    return ret;
}

/*
* write GPIO
* params: gpioId, newValue
* return: 0: success, otherwise failed
*/
static int setGpioStatus(gpioInfo_t *info, int gpioStatus)
{
    int ret = -1;
    LOGI("Set gpio %d, value %d\r\n", info->id, gpioStatus);
    ret = gpiod_line_set_value(info->lineIO, gpioStatus);
    if (ret == -1) {
        LOGI("Set gpio failed, error %d - %s\r\n", errno, strerror(errno));
    }
    return ret;
}

/*
* read GPIO
* params: gpioId
* return: gpioState: ok, < 0 failed
*/
static int getGpioStatus(gpioInfo_t* info)
{
    int val;
    val = gpiod_line_get_value(info->lineIO);
    if (val == -1) {
        LOGE("GPIO get line %d value, error no %d, mean %s\r\n", info->id, errno, strerror(errno));
    }
    return val;
}

jlong Java_com_example_gpio_Gpio_openGpio
	(JNIEnv *env, jobject clazz, jint gpioId, jint mode)
{
	//LOGI("getGpioStatus %d", gpioId);
	jlong ret = openGpio(gpioId, mode);
	return ret;
}

jint Java_com_example_gpio_Gpio_closeGpio
	(JNIEnv *env, jobject clazz, jlong info)
{
	//LOGI("getGpioStatus %d", gpioId);
	int ret = closeGpio((gpioInfo_t*)info);
	return ret;
}

jint Java_com_example_gpio_Gpio_getGpioStatus
	(JNIEnv *env, jobject clazz, jlong info)
{
	//LOGI("getGpioStatus %d", gpioId);
	int ret = getGpioStatus((gpioInfo_t*)info);
	return ret;
}

/*
* Java_com_binhanh_periph_gpio_Gpio_setGpioStatus
* params: (I)gpioId, (I)gpioStatus
* return: (I)
*/
jint Java_com_example_gpio_Gpio_setGpioStatus
	(JNIEnv *env, jobject clazz, jlong info, jint gpioStatus)
{
	int ret = setGpioStatus((gpioInfo_t*)info, gpioStatus);
	return ret;
}

/*
* Java_com_binhanh_periph_gpio_Gpio_setGpioStatus
* params: (I)gpioId, (I)gpioStatus
* return: (I)
*/
jint Java_com_example_gpio_Gpio_setMode
	(JNIEnv *env, jobject clazz, jlong info, jint mode)
{
	int ret = setMode((gpioInfo_t*)info, mode);
	return ret;
}

