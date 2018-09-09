package com.redleaves.serialport;

//public class StopBits {
//    public static final int One = 1;
//    public static final int Two = 2;
//}

import lombok.Getter;

public enum StopBits {
    One(1),
    Two(2);

    @Getter
    private final int value;

    StopBits(int value) {
        this.value = value;
    }
}