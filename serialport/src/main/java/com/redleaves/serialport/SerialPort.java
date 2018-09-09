package com.redleaves.serialport;

import android.util.Log;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.LineNumberReader;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;
import java.util.stream.Stream;

import lombok.Getter;
import lombok.Setter;

public class SerialPort {
    private static final String TAG = "SerialPort";

    static {
        System.loadLibrary("serial-port-lib");
    }

    public static class Driver {
        public Driver(String name, String root) {
            mDriverName = name;
            mDeviceRoot = root;
        }

        private String mDriverName;
        private String mDeviceRoot;
        Vector<File> mDevices = null;

        public Vector<File> getDevices() {
            if (mDevices == null) {
                mDevices = new Vector<File>();
                File dev = new File("/dev");
                File[] files = dev.listFiles();
                List fileList = Arrays.asList(files);
                Collections.sort(fileList, new Comparator<File>() {
                    @Override
                    public int compare(File o1, File o2) {
                        if (o1.isDirectory() && o2.isFile())
                            return -1;
                        if (o1.isFile() && o2.isDirectory())
                            return 1;
                        return o1.getName().compareTo(o2.getName());
                    }
                });
                for (File file : files) {
                    if (file.getAbsolutePath().startsWith(mDeviceRoot)) {
                        Log.d(TAG, "Found new device: " + file);
                        mDevices.add(file);
                    }
                }
            }
            return mDevices;
        }

        public String getName() {
            return mDriverName;
        }
    }

    private static Vector<Driver> mDrivers = null;

    private static Vector<Driver> getDrivers() throws IOException {
        if (mDrivers == null) {
            mDrivers = new Vector<Driver>();
            LineNumberReader r = new LineNumberReader(new FileReader("/proc/tty/drivers"));
            String l;
            while ((l = r.readLine()) != null) {
                // Issue 3:
                // Since driver name may contain spaces, we do not extract driver name with split()
                String drivername = l.substring(0, 0x15).trim();
                String[] w = l.split(" +");
                if ((w.length >= 5) && (w[w.length - 1].equals("serial"))) {
                    Log.d(TAG, "Found new driver " + drivername + " on " + w[w.length - 4]);
                    mDrivers.add(new Driver(drivername, w[w.length - 4]));
                }
            }
            r.close();
        }
        return mDrivers;
    }

    public static String[] getAllDevices() {
        Vector<String> devices = new Vector<String>();
        // Parse each driver
        Iterator<Driver> itdriv;
        try {
            itdriv = getDrivers().iterator();
            while (itdriv.hasNext()) {
                Driver driver = itdriv.next();
                for (File file : driver.getDevices()) {
                    String device = file.getName();
                    String value = String.format("%s (%s)", device, driver.getName());
                    devices.add(value);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return devices.toArray(new String[devices.size()]);
    }

    public static String[] getAllDevicesPath() {
        Vector<String> devices = new Vector<String>();
        // Parse each driver
        Iterator<Driver> itdriv;
        try {
            itdriv = getDrivers().iterator();
            while (itdriv.hasNext()) {
                Driver driver = itdriv.next();
                for (File file : driver.getDevices()) {
                    String device = file.getAbsolutePath();
                    devices.add(device);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return devices.toArray(new String[devices.size()]);
    }

    public final int infiniteTimeout = -1;

    public SerialPort() {
        this("/dev/ttyUSB0");
    }

    public SerialPort(String path) {
        this(path, BaudRate.B9600);
    }

    public SerialPort(String path, BaudRate baudRate) {
        this(path, baudRate, Parity.None);
    }

    public SerialPort(String path, BaudRate baudRate, Parity parity) {
        this(path, baudRate, parity, DataBits.Eight);
    }

    public SerialPort(String path, BaudRate baudRate, Parity parity, DataBits dataBits) {
        this(path, baudRate, parity, dataBits, StopBits.One);
    }

    public SerialPort(String path, BaudRate baudRate, Parity parity, DataBits dataBits, StopBits stopBits) {
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

    public void setBaudRate(BaudRate baudRate) {
        this.baudRate = baudRate;
        if (isOpen) {
            serial_port_set_baudrate(fd, baudRate.getValue());
        }
    }

    @Getter
    protected BaudRate baudRate;

    public void setDataBits(DataBits dataBits) {
        this.dataBits = dataBits;
        if (isOpen) {
            serial_port_set_databits(fd, dataBits.getValue());
        }
    }

    @Getter
    protected DataBits dataBits;

    public void setParity(Parity parity) {
        this.parity = parity;
        if (isOpen) {
            serial_port_set_parity(fd, parity.getValue());
        }
    }

    @Getter
    protected Parity parity;

    public void setStopBits(StopBits stopBits) {
        this.stopBits = stopBits;
        if (isOpen) {
            serial_port_set_stopbits(fd, stopBits.getValue());
        }
    }

    @Getter
    protected StopBits stopBits;

    private FileDescriptor fd = null;
    private InputStream inputStream;
    private OutputStream outputStream;

//    public event SerialErrorReceivedEventHandler ErrorReceived;
//    public event SerialDataReceivedEventHandler DataReceived;
//    public event SerialPinChangedEventHandler PinChanged;

    public void discardInBuffer() {

    }

    public void discardOutBuffer() {

    }

    public boolean open() {
        if (!isOpen) {
            fd = serial_port_open(path);
            if (fd != null) {
                isOpen = true;
                serial_port_set_baudrate(fd, baudRate.getValue());
                serial_port_set_databits(fd, dataBits.getValue());
                serial_port_set_parity(fd, parity.getValue());
                serial_port_set_stopbits(fd, stopBits.getValue());
                inputStream = new FileInputStream(fd);
                outputStream = new FileOutputStream(fd);
            }
        }
        return isOpen;
    }

    public boolean close() {
        if (isOpen) {
            if (serial_port_close(fd)) {
                fd = null;
                isOpen = false;
                try {
                    inputStream.close();
                    outputStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return !isOpen;
    }

    public int read(byte[] buffer, int offset, int count) throws IOException {
        if (isOpen) {
            return inputStream.read(buffer, offset, count);
        }
        return 0;
    }

    public int readByte() {
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
            outputStream.flush();
        }
    }

    public void write(String text) throws IOException {
        byte[] data = text.getBytes();
        write(data, 0, data.length);
    }

    public void writeLine(String text) {

    }

//    protected override void Dispose(bool disposing);


    protected native FileDescriptor serial_port_open(String path);

    protected native boolean serial_port_close(FileDescriptor fd);

    protected native void serial_port_set_baudrate(FileDescriptor fd, int baudRate);

    protected native void serial_port_set_databits(FileDescriptor fd, int dataBits);

    protected native void serial_port_set_parity(FileDescriptor fd, int parity);

    protected native void serial_port_set_stopbits(FileDescriptor fd, int stopBits);
}