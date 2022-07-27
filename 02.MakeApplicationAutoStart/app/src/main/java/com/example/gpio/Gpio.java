package com.example.gpio;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import com.example.gpio.R;
// Java_com_binhanh_periph_gpio_Gpio_
import android.util.Log;

public class Gpio extends AppCompatActivity {
    /* this is used to load the 'hello-jni' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.hellojni/lib/libhello-jni.so at
     * installation time by the package manager.
     */
    private static final boolean compileNative = true;

    static {
        System.loadLibrary("gpio");
    }

    private static final String TAG = "MainActivityGpio";

    private class MyRunnable implements Runnable{
        private int counter = 0;
        @Override
        public void run() {
            while (true) {
                try {
                    if (compileNative) {
                        setGpioStatus(942, getGpioStatus(942) > 0 ? 0 : 1);
                        Log.i(TAG, "GPIO=" + getGpioStatus(942));
                    }
                    Thread.sleep(500);
                    counter++;
                    if (counter > 8)
                    {
                        if (compileNative) {
                            setGpioStatus(942, 0);
                        }
                        break;
                    }
                } catch (InterruptedException e) {
                    Log.e(TAG, "IO FAILED");
                    e.printStackTrace();
                }
            }
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (compileNative) {
            setGpioStatus(942, 0);
        }
        new Thread(new MyRunnable()).start();
    }
    public native int setGpioStatus(int gpioId, int gpioStatus);
    public native int getGpioStatus(int gpioId);
}