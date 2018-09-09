package com.redleaves.serialport;

//public class Parity {
//    public static final int None = 0;
//    public static final int Odd = 1;
//    public static final int Even = 2;
//    public static final int Mark = 3;
//    public static final int Space = 4;
//}

import lombok.Getter;

public enum Parity {
    None(0),
    Odd(1),
    Even(2),
    Mark(3),
    Space(4);

    @Getter
    private final int value;

    Parity(int value) {
        this.value = value;
    }
}