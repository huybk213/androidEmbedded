package com.example.gpio.Gpio;
public class Gpio {
    static {
        System.loadLibrary("gpio");
    }

    public Gpio() {

    }

    public long newIO(int io)
    {
        return createIO(io);
    }

    // C
   public native int setGpioStatus(long gpioObj, int gpioStatus);
//    public native int getGpioStatus(int gpioId);
    // CPP
    public native int setOutputMode(long gpioObj);
    public native long createIO(int gpioId);
    public native void deleteIO(long gpioObj);
}