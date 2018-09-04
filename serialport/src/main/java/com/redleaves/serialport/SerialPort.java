package com.redleaves.serialport;

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.stream.Stream;

import lombok.Getter;
import lombok.Setter;

public class SerialPort {
    static {
        System.loadLibrary("serial-port-lib");
    }

    public final int infiniteTimeout = -1;

    public SerialPort() {
        new SerialPort("/dev/ttyUSB0");
    }

    public SerialPort(String path) {
        new SerialPort(path, 9600);
    }

    public SerialPort(String path, int baudRate) {
        new SerialPort(path, baudRate, Parity.None);
    }

    public SerialPort(String path, int baudRate, int parity) {
        new SerialPort(path, baudRate, parity, DataBits.Eight);
    }

    public SerialPort(String path, int baudRate, int parity, int dataBits) {
        new SerialPort(path, baudRate, parity, dataBits, StopBits.One);
    }

    public SerialPort(String path, int baudRate, int parity, int dataBits, int stopBits) {
        this.path = path;
        this.baudRate = baudRate;
        this.parity = parity;
        this.dataBits = dataBits;
        this.stopBits = stopBits;
    }


//    @Getter
//    @Setter
//    protected Handshake handshake;

//    @Getter
//    @Setter
//    protected Encoding encoding;

    @Getter
    @Setter
    protected boolean dtrEnable;

    @Getter
    protected boolean ctsHolding;

    @Getter
    @Setter
    protected boolean discardNull;

    @Getter
    protected boolean isOpen = false;

    @Getter
    protected boolean dsrHolding;

    @Getter
    @Setter
    protected String newLine;

    @Getter
    @Setter
    protected int readBufferSize;

    @Getter
    @Setter
    protected byte parityReplace;

    @Getter
    @Setter
    protected String path;

    @Getter
    protected boolean cdHolding;

    @Getter
    @Setter
    protected int readTimeout;

    @Getter
    @Setter
    protected int receivedBytesThreshold;

    @Getter
    @Setter
    protected boolean rtsEnable;

    @Getter
    @Setter
    protected int writeBufferSize;

    @Getter
    @Setter
    protected int writeTimeout;

    @Getter
    protected int bytesToRead;

    @Getter
    @Setter
    protected boolean breakState;

    @Getter
    protected Stream baseStream;

    @Getter
    protected int bytesToWrite;

    public void setBaudRate(int baudRate) {
        this.baudRate = baudRate;
        serial_port_set_baudrate(fd, baudRate);
    }

    @Getter
    protected int baudRate;

    public void setDataBits(int dataBits) {
        this.dataBits = dataBits;
        serial_port_set_databits(fd, dataBits);
    }

    @Getter
    protected int dataBits;

    public void setParity(int parity) {
        this.parity = parity;
        serial_port_set_parity(fd, parity);
    }

    @Getter
    protected int parity;

    public void setStopBits(int stopBits) {
        this.stopBits = stopBits;
        serial_port_set_stopbits(fd, stopBits);
    }

    @Getter
    protected int stopBits;




    private FileDescriptor fd = null;
    private InputStream inputStream;
    private OutputStream outputStream;

//    public event SerialErrorReceivedEventHandler ErrorReceived;
//    public event SerialDataReceivedEventHandler DataReceived;
//    public event SerialPinChangedEventHandler PinChanged;


    public static String[] getPaths() {
        return new String[0];
    }

    public void close() {
        if(isOpen) {
            serial_port_close(fd);
            fd = null;
            isOpen = false;
        }
    }

    public void discardInBuffer() {

    }

    public void discardOutBuffer() {

    }

    public void open() {
        if(!isOpen) {
            fd = serial_port_open(path);
            if (fd != null) {
                isOpen = true;
            }
            serial_port_set_baudrate(fd, baudRate);
            serial_port_set_databits(fd, dataBits);
            serial_port_set_parity(fd, parity);
            serial_port_set_stopbits(fd, stopBits);
            inputStream = new FileInputStream(fd);
            outputStream = new FileOutputStream(fd);
        }
    }

    public int read(byte[] buffer, int offset, int count) throws IOException {
        if (isOpen) {
            return inputStream.read(buffer, offset, count);
        }
        return -1;
    }

    public int read(char[] buffer, int offset, int count) {
        return 0;
    }

    public int readByte() {
        return 0;
    }

    public int readChar() {
        return 0;
    }

    public String readExisting() {
        return new String();
    }

    public String readLine() {
        return new String();
    }

    public String readTo(String value) {
        return new String();
    }

    public void write(byte[] buffer, int offset, int count) throws IOException {
        if (isOpen) {
            outputStream.write(buffer, offset, count);
        }
    }

    public void write(String text) {

    }

    public void write(char[] buffer, int offset, int count) {

    }

    public void writeLine(String text) {

    }

//    protected override void Dispose(bool disposing);


    protected native FileDescriptor serial_port_open(String path);

    protected native void serial_port_close(FileDescriptor fd);

    protected native void serial_port_set_baudrate(FileDescriptor fd, int baudRate);

    protected native void serial_port_set_databits(FileDescriptor fd, int dataBits);

    protected native void serial_port_set_parity(FileDescriptor fd, int parity);

    protected native void serial_port_set_stopbits(FileDescriptor fd, int stopBits);
}
