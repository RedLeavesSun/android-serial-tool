package com.redleaves.serialport;

//public class DataBits {
//    public static final int Five = 5;
//    public static final int Six = 6;
//    public static final int Seven = 7;
//    public static final int Eight = 8;
//}

import lombok.Getter;

public enum DataBits {
    Five(5),
    Six(6),
    Seven(7),
    Eight(8);

    @Getter
    private final int value;

    DataBits(int value) {
        this.value = value;
    }
}