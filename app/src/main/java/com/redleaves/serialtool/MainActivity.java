package com.redleaves.serialtool;

import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.redleaves.serialport.BaudRate;
import com.redleaves.serialport.DataBits;
import com.redleaves.serialport.Parity;
import com.redleaves.serialport.SerialPort;
import com.redleaves.serialport.StopBits;
import com.redleaves.serialtool.Utils.Utils;

import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Locale;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, AdapterView.OnItemSelectedListener {
    private static final String TAG = "SerialTool";

    private static final int SERIAL_RECE = 0;

    private class MainHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case SERIAL_RECE: {
                    String rece = (String) msg.obj;
                    SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss", Locale.CHINA);
                    String date = simpleDateFormat.format(new java.util.Date());
                    textRece.append(String.format("%s: %s\n", date, rece));
                    int offset = textRece.getLineCount() * textRece.getLineHeight();
                    if (offset > textRece.getHeight()) {
                        textRece.setScrollY(offset - textRece.getHeight());
                    }
                    break;
                }
            }
        }
    }

    private class SerialThead extends Thread {
        @Override
        public void run() {
            Log.d(TAG, "thread is launch...");
            while (!isInterrupted()) {
                if (serialPort != null && serialPort.isOpen()) {
                    try {
                        byte[] rece = new byte[64];
                        int size = serialPort.read(rece, 0, rece.length);
                        byte[] data = new byte[size];
                        System.arraycopy(rece, 0, data, 0, size);
                        if (data.length != 0)
                            Log.d(TAG, String.format("rece: %s", Utils.bytesToHex(data, size)));
                        if (data.length != 0) {
                            Message msg = new Message();
                            msg.what = SERIAL_RECE;
                            msg.obj = Utils.bytesToHex(data, size);
                            handler.sendMessage(msg);
                        }
                        Thread.sleep(1);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
            Log.d(TAG, "thread is quiting...");
        }

        @Override
        public void interrupt() {
            Log.d(TAG, "thread was interrupted...");
            super.interrupt();
        }
    }

    private MainHandler handler = new MainHandler();

    private Spinner spinnerSerialPort;
    private Spinner spinnerBaudRate;
    private Spinner spinnerDataBits;
    private Spinner spinnerParity;
    private Spinner spinnerStopBits;

    private Button buttonScan;
    private Button buttonOpen;
    private Button buttonClose;
    private Button buttonSend;

    private EditText textSend;
    private TextView textRece;

    private SerialPort serialPort = new SerialPort();
    private Thread thread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        spinnerSerialPort = findViewById(R.id.spinner_serialport);
        spinnerBaudRate = findViewById(R.id.spinner_baudrate);
        spinnerDataBits = findViewById(R.id.spinner_databits);
        spinnerParity = findViewById(R.id.spinner_patiry);
        spinnerStopBits = findViewById(R.id.spinner_stopbits);

        spinnerSerialPort.setOnItemSelectedListener(this);
        spinnerBaudRate.setOnItemSelectedListener(this);
        spinnerDataBits.setOnItemSelectedListener(this);
        spinnerParity.setOnItemSelectedListener(this);
        spinnerStopBits.setOnItemSelectedListener(this);

        buttonScan = findViewById(R.id.scan);
        buttonOpen = findViewById(R.id.open);
        buttonClose = findViewById(R.id.close);
        buttonSend = findViewById(R.id.send);

        buttonScan.setOnClickListener(this);
        buttonOpen.setOnClickListener(this);
        buttonClose.setOnClickListener(this);
        buttonSend.setOnClickListener(this);

        textSend = findViewById(R.id.text_send);
        textRece = findViewById(R.id.text_rece);


        buttonOpen.setEnabled(true);
        buttonClose.setEnabled(false);
        textRece.setMovementMethod(new ScrollingMovementMethod());

        makeSpinnerBaudRateList();
        makeSpinnerDataBits();
        makeSpinnerParity();
        makeSpinnerStopBits();

        spinnerBaudRate.setSelection(BaudRate.B9600.ordinal());
        spinnerDataBits.setSelection(DataBits.Eight.ordinal());
        spinnerParity.setSelection(Parity.None.ordinal());
        spinnerStopBits.setSelection(StopBits.One.ordinal());

        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_STATE_HIDDEN);

        onClick(buttonScan);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.scan: {
                String[] paths = SerialPort.getAllDevicesPath();
                ArrayList<String> list = new ArrayList<>(Arrays.asList(paths));
                ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, list);
                adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                spinnerSerialPort.setAdapter(adapter);
                break;
            }
            case R.id.open: {
                if (!serialPort.isOpen()) {
                    serialPort.setPath(spinnerSerialPort.getSelectedItem().toString());
                    if (serialPort.open()) {
                        thread = new SerialThead();
                        thread.start();
                        buttonOpen.setEnabled(false);
                        buttonClose.setEnabled(true);
                        Toast.makeText(this, "Open serial port success", Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(this, "Open serial port fail", Toast.LENGTH_SHORT).show();
                    }
                }
                break;
            }
            case R.id.close: {
                if (serialPort.isOpen()) {
                    if (serialPort.close()) {
                        if (thread.isAlive()) {
                            thread.interrupt();
                        }
                        buttonOpen.setEnabled(true);
                        buttonClose.setEnabled(false);
                        Toast.makeText(this, "Close serial port success", Toast.LENGTH_SHORT).show();
                    } else {
                        Toast.makeText(this, "Close serial port fail", Toast.LENGTH_SHORT).show();
                    }
                }
                break;
            }
            case R.id.send: {
                if (serialPort.isOpen()) {
                    try {
                        String text = textSend.getText().toString();
                        if (!text.equals("")) {
                            Log.d(TAG, String.format("send: %s", Utils.bytesToHex(text.getBytes(), text.getBytes().length)));
                            serialPort.write(text);
                        } else {
                            Toast.makeText(this, "Content is empty", Toast.LENGTH_SHORT).show();
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                } else {
                    Toast.makeText(this, "Please open a serial port", Toast.LENGTH_SHORT).show();
                }
                break;
            }
        }
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
        switch (parent.getId()) {
            case R.id.spinner_serialport: {
                serialPort.setPath((String) spinnerSerialPort.getSelectedItem());
                break;
            }
            case R.id.spinner_baudrate: {
                serialPort.setBaudRate((BaudRate) spinnerBaudRate.getSelectedItem());
                break;
            }
            case R.id.spinner_databits: {
                serialPort.setDataBits((DataBits) spinnerDataBits.getSelectedItem());
                break;
            }
            case R.id.spinner_patiry: {
                serialPort.setParity((Parity) spinnerParity.getSelectedItem());
                break;
            }
            case R.id.spinner_stopbits: {
                serialPort.setStopBits((StopBits) spinnerStopBits.getSelectedItem());
                break;
            }
        }
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) {

    }

    private void makeSpinnerBaudRateList() {
        ArrayList<BaudRate> list = new ArrayList<>(Arrays.asList(BaudRate.values()));
        ArrayAdapter<BaudRate> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, list);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinnerBaudRate.setAdapter(adapter);
    }

    private void makeSpinnerDataBits() {
        ArrayList<DataBits> list = new ArrayList<>(Arrays.asList(DataBits.values()));
        ArrayAdapter<DataBits> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, list);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinnerDataBits.setAdapter(adapter);
    }

    private void makeSpinnerParity() {
        ArrayList<Parity> list = new ArrayList<>(Arrays.asList(Parity.values()));
        ArrayAdapter<Parity> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, list);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinnerParity.setAdapter(adapter);
    }

    private void makeSpinnerStopBits() {
        ArrayList<StopBits> list = new ArrayList<>(Arrays.asList(StopBits.values()));
        ArrayAdapter<StopBits> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, list);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinnerStopBits.setAdapter(adapter);
    }
}
