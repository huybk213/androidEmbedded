package com.example.minnative;

public class MinNative {

    static {
        System.loadLibrary("minnative");
    }
    public native byte[] minBuildRawFrameOutput(int id, byte[] data);

    public byte[] buildFrame(int id, byte[] data)
    {
        return minBuildRawFrameOutput(id, data);
    }
}
