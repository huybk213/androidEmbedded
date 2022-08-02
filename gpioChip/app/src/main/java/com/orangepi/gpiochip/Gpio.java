package com.orangepi.gpiochip;
import android.util.Log;

public class Gpio {
    private static final String TAG = "GpioActive";

    static {
        Log.i(TAG, "Load gpio lib\r\n");
        System.loadLibrary("gpio");
    }

    static private  int mIoNumber;
    static private  long mIoObj;
    public Gpio(int number) {
        mIoNumber = number;
        mIoObj = CreateIO(number);
    }

    public void releaseIO()
    {
        deleteIO(mIoObj);
        mIoObj = 0;
        mIoNumber = -1;
    }

    public int configOutputMode()
    {
        return setOutputMode(mIoObj);
    }

    public int writeOutput(int value)
    {
        return setGpioStatus(mIoObj, value);
    }

    // C
    public native int setGpioStatus(long gpioObj, int gpioStatus);
    //    public native int getGpioStatus(int gpioId);
    // CPP
    public native int setOutputMode(long gpioObj);
    public native long CreateIO(int gpioId);
    public native void deleteIO(long gpioObj);
}
