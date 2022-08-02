package com.example.gpio;
import android.util.Log;

public class Gpio {
    private static final String TAG = "GpioActive";
    public enum gpioMode {
        MODE_OUTPUT,
        MODE_INPUT,
//        MODE_ANALOG_INPUT
    }

    static {
        Log.i(TAG, "Load gpio lib\r\n");
        System.loadLibrary("gpio");
    }

    static private  int mIoNumber;
    gpioMode mMode;

    public Gpio(int number, gpioMode mode) {
        mIoNumber = number;
        mMode = mode;
        createIO();
    }

    public int releaseGpio()
    {
        int tmp = mIoNumber;
        mIoNumber = -1;
        return closeGpio(mIoNumber);
    }

    public int createIO() {
        int mode = 0;
        if (mMode == gpioMode.MODE_INPUT) {
            mode = 1;
        }
//        else if (mMode == gpioMode.MODE_ANALOG_INPUT) {
//            mode = 2;
//        }
        if (openGpio(mIoNumber, mode) == -1) {
            Log.e(TAG, "Open Gpio " + mIoNumber + " failed\r\n");
            return -1;
        } else {
            return 0;
        }
    }

    public int setGpioMode(gpioMode mode)
    {
        int tmp = 0;
        if (mMode == gpioMode.MODE_INPUT) {
            tmp = 1;
        }
        if (setMode(mIoNumber, tmp) != -1) {
            mMode = mode;
            return 0;
        }
        Log.e(TAG, "Set mode failed\r\n");
        return -1;
    }

    public gpioMode getGpioMode()
    {
        return mMode;
    }

    public int writeOutput(int value)
    {
        return setGpioStatus(mIoNumber, value);
    }

    public int readOutput(int value)
    {
        return getGpioStatus(mIoNumber);
    }

    public int readInput()
    {
        return getGpioStatus(mIoNumber);
    }

    // C
    public native int setGpioStatus(int gpioObj, int gpioStatus);
    public native int getGpioStatus(int gpioObj);

    public native int setMode(int gpioId, int mode);
    public native int openGpio(int gpioId, int mode);
    public native int closeGpio(int gpioId);
    public native void deleteIO(long gpioObj);
}
