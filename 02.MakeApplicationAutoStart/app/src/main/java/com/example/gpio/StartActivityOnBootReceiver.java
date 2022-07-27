package com.example.gpio;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class StartActivityOnBootReceiver extends BroadcastReceiver {
    public void onReceive(Context context, Intent intent) {
        String action = intent.getAction();
        Log.i("StartActivityOnBootReceiver", action);
        if (Intent.ACTION_BOOT_COMPLETED.equals(action)) {
            Intent i = new Intent(context, Gpio.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            context.startActivity(i);
        }
    }
}
