package com.orangepi.gpiochip;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;

import java.io.BufferedReader;
import java.io.InputStreamReader;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        new Thread(new MyRunnable()).start();
    }


    public static final int baCamBuzzerPin = 942;
    public static final int orangePi3PC11Pin = 75;      // 32* ('leter' - 'A') + num
    private static final String TAG = "MainActivityGpio";
    Gpio mMyGpio; // = new Gpio(orangePi3PC11Pin);


    private class MyRunnable implements Runnable{
        private int counter = 0;
        @Override
        public void run() {
            if (!runShellCmd("chmod 777 /dev/gpiochip*"))
            {
                Log.e(TAG, "Set permission for gpio failed\r\n");
            }
            else
            {
                Log.i(TAG, "Set permission for gpio success\r\n");
            }
            int value = 0;
            mMyGpio = new Gpio(orangePi3PC11Pin);
            mMyGpio.configOutputMode();
//            setOutputMode(gpio75);
            while (true) {
                try {
//                    int gpioCurrentValue = getGpioStatus(orangePi3PC11Pin);
//                    if (gpioCurrentValue != -1) {
//                        setGpioStatus(orangePi3PC11Pin, getGpioStatus(orangePi3PC11Pin) > 0 ? 0 : 1);
//                        Log.i(TAG, "GPIO=" + getGpioStatus(orangePi3PC11Pin));
//                        Thread.sleep(2000);
//                        counter++;
//                        if (counter > 10000) {
//                            setGpioStatus(orangePi3PC11Pin, 0);
//                            break;
//                        }
//                    }
//                    else {
//                        Log.e(TAG, "Get orange pi gpio " + orangePi3PC11Pin + " error");
//                        break;
//                    }
                    value = value > 0 ? 0 : 1;
//                    setGpioStatus(gpio75, value);
                    mMyGpio.writeOutput(value);
                    Log.i(TAG, "GPIO=" + value);
                    Thread.sleep(2000);
                } catch (InterruptedException e) {
//                    deleteIO(gpio75);
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * Chạy lệnh shell
     * @param cmd
     */
    public static boolean runShellCmd(String cmd) {
        try {
            Process proc = Runtime.getRuntime().exec(new String[]{"su", "-c", cmd});
            proc.waitFor();
            BufferedReader input = new BufferedReader(new InputStreamReader(proc.getErrorStream()));
            String line;
            boolean hasError = false;
            while ((line = input.readLine()) != null) {
                if (line.contains("Failure")) {
                    Log.e(TAG, "input: " + line);
                    hasError = true;
                }
            }
            if (proc.exitValue() != 0 || hasError) {
                Log.e(TAG, "run shell failed: " + proc.exitValue() + ", hasErr: " + hasError);
                return false;
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }
}