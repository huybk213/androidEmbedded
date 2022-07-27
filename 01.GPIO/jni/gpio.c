
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "gpio.h"

#define DEVICE_NAME_ADC	"/sys/class/power_supply/battery/mpp4_voltage"
#define ERROR  -1

// static void throw_NullPointerException(JNIEnv *env, const char* msg)
// {
    // jclass clazz;
    // clazz = env->FindClass("java/lang/NullPointerException");
    // env->ThrowNew(clazz, msg);
// }

/*
* write GPIO
* params: gpioId, newValue
* return: 0: success, otherwise failed
*/
static int setGpioStatus(jint gpioId, jint gpioStatus)
{
    int fd = -1;
	
	char gpioName[35] = {0};
	int index = sprintf(gpioName, "/sys/class/gpio/gpio%d", gpioId);
	sprintf(&gpioName[index], "/value");
	
	fd = open(gpioName, O_RDWR|O_NONBLOCK);
    
    if(fd < 0){
       LOGE("gpio%d open failure", gpioId);
       return ERROR;
    }
	

    char dataBuff[1];	
    if(gpioStatus == 0){
        dataBuff[0] = '0';
    } else if(gpioStatus == 1){
        dataBuff[0] = '1';
    } else {
        return -1;
    }
    int writeResult = write(fd, dataBuff, 1);
    if(writeResult <= 0){
        LOGE("gpio%d write failure", gpioId);
		return ERROR;
    }
    //LOGI("set gpio%d status: %d", gpioId, gpioStatus);
	
    close(fd);
	return 0;
}

/*
* read GPIO
* params: gpioId
* return: gpioState: ok, < 0 failed
*/
static int getGpioStatus(jint gpioId) 
{
    char readBuff[1];
	int fd = -1;
    int readResult = -1;

	char gpioName[35] = {0};
	int index = sprintf(gpioName, "/sys/class/gpio/gpio%d", gpioId);
	sprintf(&gpioName[index], "/value");
	
    
	fd = open(gpioName, O_RDWR|O_NONBLOCK);
    if(fd < 0){
       LOGE("gpio%d open failure", gpioId);
       return ERROR;
    }
	
    int readInfo = 0;
    readResult = read(fd, readBuff, 1);
    if(readResult > 0){
        readInfo = readBuff[0];
        if(readInfo >= '0')
			readInfo = readInfo - '0';
    }
	else
	{
        LOGE("gpio%d read failure", gpioId);
		return ERROR;
	}
	
    close(fd);
    return readInfo;
}

/*
* getAdc value
* params: none
* return: adcValue: ok, < 0 failed
*/
static long int getAdcStatus(void)
{
	char readBuff[8];
	int buffSize = 8;
	int readResult = -1;
	int fd = -1;
	long int readInfo = 0;
	
	fd = open(DEVICE_NAME_ADC, O_RDWR|O_NONBLOCK);
	if(fd < 0)
	{
		LOGE("adc open failure\n");
		return ERROR;
	}
	readResult = read(fd, readBuff, buffSize);
	if(readResult > 0)
	{
		readInfo = atoi(readBuff);
		LOGI("adc get readBuff=%s, readInfo=%ld\n", readBuff, readInfo);
	}
	else
	{
		LOGE("adc read failure\n");
		return ERROR;
	}
	
	close(fd);
	return readInfo;
}

// static JNINativeMethod method_table[] = {
		// { "getStatus_native", "(I)I", (void*)setGpioStatus },
		// { "setStatus_native", "(II)V", (void*)getGpioStatus },
// };

#if 0
/*
* Java_com_binhanh_periph_gpio_Gpio_getGpioStatus
* params: (I)gpioId
* return: (I)
*/
jint Java_com_binhanh_periph_gpio_Gpio_getGpioStatus
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
jint Java_com_binhanh_periph_gpio_Gpio_setGpioStatus
	(JNIEnv *env, jobject clazz, jint gpioId, jint gpioStatus)
{
	int ret = setGpioStatus(gpioId, gpioStatus);
	return ret;
}

/*
* Java_com_binhanh_periph_gpio_Gpio_getAdcStatus
* params: (I)gpioId, (I)gpioStatus
* return: (I)
*/     
jint Java_com_binhanh_periph_gpio_Gpio_getAdcStatus
	(JNIEnv *env, jobject clazz)
{
	int ret = getAdcStatus();
	return ret;
}

#endif

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
* Java_com_binhanh_periph_gpio_Gpio_getAdcStatus
* params: (I)gpioId, (I)gpioStatus
* return: (I)
*/
jint Java_com_example_gpio_Gpio_getAdcStatus
	(JNIEnv *env, jobject clazz)
{
	int ret = getAdcStatus();
	return ret;
}
