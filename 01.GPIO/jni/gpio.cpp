#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
// #include <linux/gpio.h>
#include "gpiod.h"
#include "bytGpio.h"

bytGpioApi::bytGpioApi()
{

}

int bytGpioApi::begin(int pin)
{
    int ret = -1, val;
    // Orangepi3 gpiochip0 (0-64)
    if (pin > 64)
    {
        chipname = "gpiochip1";
    }
    else
    {
        this->chipname = "gpiochip0";
    }
    this->pin = pin;
    sprintf(this->name, "%s_%d", chipname, pin);

    LOGI("Select %s, pin %d\r", chipname, pin);

    // Open GPIO chip
    chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
		LOGE("Open chip %s io %d failed, error %d, mean %s\r\n", chipname, pin, errno, strerror(errno));
		goto end;
	}

    // Open GPIO lines
    lineIO = gpiod_chip_get_line(chip, pin);
    if (!lineIO) {
		LOGE("Get line failed\r\n");
		goto end;
	}
    this->status = 0;
    ret = 0;
end:
    return 1;
}

bytGpioApi::~bytGpioApi()
{
    gpiod_line_release(lineIO);
    gpiod_chip_close(chip);
    this->status = 0;
    this->lineIO = NULL;
    this->chip = NULL;
    memset(this->name, 0, 128);
}

int bytGpioApi::gpioSetModeOutput()
{
    // Open lines for output
    int ret = -1;
    ret = gpiod_line_request_output(this->lineIO, this->name, this->status);
    if (ret < 0) {
        LOGE("Request line as output failed %d\r\n", errno);
    }
    return ret;
}

int bytGpioApi::gpioWriteOutput(int value)
{
    int ret = -1;

    // Set value
    ret = gpiod_line_set_value(this->lineIO, value);
    if (ret == 0) {
        this->status = value;
    }
    else {
        LOGE("Set gpio %d FAILED, error %d, mean %s\r\n", this->pin, errno, strerror(errno));
    }
    return ret;
}

int bytGpioApi::gpioReadOutput()
{
#if 0
    int ret = -1;
    // Set value
    ret = gpiod_line_get_value(this->lineIO);
    if (ret == 0) {
        this->status = value;
    }
    else {
        LOGE("Read gpio %d FAILED, error %d, mean %s\r\n", this->pin, errno, strerror(errno));
    }
    return ret;
#else
    return this->status;
#endif
}

int bytGpioApi::gpioSetInput()
{
    return -1;
}

int bytGpioApi::gpioReadInput()
{
    return -1;
}


// /*
// * read GPIO
// * params: gpioId
// * return: gpioState: ok, < 0 failed
// */
// static int getGpioStatus(jint gpioId)
// {
//     const char *chipname = "gpiochip0";
//     struct gpiod_chip *chip;
//     struct gpiod_line *lineIO;
//     int val = -1;

//     // Open GPIO chip
//     // Orangepi3 gpiochip0 (0-64)
//     if (gpioId > 64)
//     {
//         chipname = "gpiochip1";
//     }
//     chip = gpiod_chip_open_by_name(chipname);
//     if (!chip) {
// 		LOGE("Open chip %s io %d failed, error %d, mean %s\r\n", chipname, gpioId, errno, strerror(errno));
// 		goto end;
// 	}

//     // Open GPIO lines
//     lineIO = gpiod_chip_get_line(chip, gpioId);
//     if (!lineIO) {
// 		LOGE("Get %s line %d failed, error %d, mean %s\r\n", chipname, gpioId, errno, strerror(errno));
// 		goto close_chip;
// 	}

// //    // Open switch line for input
// //    gpiod_line_request_input(lineIO, "gpio_state");
//     val = gpiod_line_get_value(lineIO);
//     LOGE("GPIO%d = %d\r\n", gpioId, val);
//     if (val == -1)
//     {
//         LOGE("GPIO get line value, error no %d, mean %s\r\n", errno, strerror(errno));
//     }

//     // Release lines and chip
//     gpiod_line_release(lineIO);
// close_chip:
//     gpiod_chip_close(chip);
// end:
//     return val;
// }


// jint Java_com_example_gpio_Gpio_getGpioStatus
// 	(JNIEnv *env, jobject clazz, jint gpioId)
// {
// 	//LOGI("getGpioStatus %d", gpioId);
// 	int ret = getGpioStatus(gpioId);
// 	return ret;
// }

// /*
// * Java_com_binhanh_periph_gpio_Gpio_setGpioStatus
// * params: (I)gpioId, (I)gpioStatus
// * return: (I)
// */
// jint Java_com_example_gpio_Gpio_setGpioStatus
// 	(JNIEnv *env, jobject clazz, jint gpioId, jint gpioStatus)
// {
// 	int ret = setGpioStatus(gpioId, gpioStatus);
// 	return ret;
// }


/*
* Java_com_binhanh_periph_gpio_Gpio_setGpioStatus
* params: (I)gpioId, (I)gpioStatus
* return: (I)
*/
jint Java_com_example_gpio_Gpio_setGpioStatus
	(JNIEnv *env, jobject clazz, jlong gpioObj, jint value)
{    
    bytGpioApi *io = (bytGpioApi*)gpioObj;
    return io->gpioWriteOutput(value);
}


jint Java_com_example_gpio_Gpio_Gpio_setOutputMode
	(JNIEnv *env, jobject clazz, jlong gpioObj)
{
    bytGpioApi *io = (bytGpioApi*)gpioObj;
    return io->gpioSetModeOutput();
}

jlong Java_com_example_gpio_Gpio_Gpio_createIO
	(JNIEnv *env, jobject clazz, jint gpioId)
{
    bytGpioApi *ptr = new bytGpioApi;
    if (ptr)
    {
        ptr->begin(gpioId);
    }
    return (long)(ptr);
}

void Java_com_example_gpio_Gpio_Gpio_deleteIO
	(JNIEnv *env, jobject clazz, jlong ptr)
{
    // (bytGpio*)(ptr)->release();
    delete (bytGpioApi*)(ptr);
}
