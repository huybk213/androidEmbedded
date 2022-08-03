package com.example.gpio;
import android.util.Log;

public class Gpio {
    private static final String TAG = "GpioActive";
    public enum gpioMode {
        MODE_OUTPUT,
        MODE_INPUT,
    }

    static {
        Log.i(TAG, "Load gpio lib\r\n");
        System.loadLibrary("gpio");
    }

    private int mIoNumber;
    private long mIoPtr;
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
        mIoPtr = openGpio(mIoNumber, mode);
        if (mIoPtr == 0) {
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
        if (setMode(mIoPtr, tmp) != -1) {
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
        return setGpioStatus(mIoPtr, value);
    }

    public int readOutput()
    {
        return getGpioStatus(mIoPtr);
    }

    public int readInput()
    {
        return getGpioStatus(mIoPtr);
    }

    // C
    public native int setGpioStatus(long ioObj, int gpioStatus);
    public native int getGpioStatus(long ioObj);

    public native int setMode(long ioObj, int mode);
    public native long openGpio(int gpioId, int mode);
    public native int closeGpio(long ioObj);
    public native void deleteIO(long gpioObj);
}
