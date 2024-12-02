package com.example.rootsofevil;

import android.app.NativeActivity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import com.example.rootsofevil.databinding.ActivityMainBinding;
public class MainActivity extends NativeActivity {
    static {
        System.loadLibrary("main");
    }

    private ActivityMainBinding binding;
    private SensorManager sensorManager;
    private Sensor accelerometer;
    private float accelX, accelY, accelZ;
    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sensorManager.registerListener(new SensorEventListener() {
            @Override
            public void onSensorChanged(SensorEvent event) {
                accelX = event.values[0];
                accelY = event.values[1];
                accelZ = event.values[2];

                passAccelerometerData(accelX, accelY, accelZ);
            }

            @Override
            public void onAccuracyChanged(Sensor sensor, int accuracy) {}
        }, accelerometer, SensorManager.SENSOR_DELAY_GAME);
    }

    public float[] getAccelerometerValues() {
        return new float[]{accelX, accelY, accelZ};
    }

    @Override
    protected void onPause() {
        super.onPause();
        onPauseNative();
    }

    @Override
    protected void onResume() {
        super.onResume();
        onResumeNative();
    }

    public static native void onPauseNative();
    public static native void onResumeNative();
    public native void passAccelerometerData(float x, float y, float z);

}
