package com.redleaves.serialtool;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.redleaves.serialport.BaudRate;
import com.redleaves.serialport.DataBits;
import com.redleaves.serialport.Parity;
import com.redleaves.serialport.SerialPort;
import com.redleaves.serialport.StopBits;

import java.io.IOException;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private static final String TAG = "SerialTool";

    SerialPort serialPort;
    Thread thread;
    Button button;
    EditText editText;
    TextView textView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        serialPort = new SerialPort("/dev/ttyS3", BaudRate.B115200, Parity.None, DataBits.Eight, StopBits.One);
        serialPort.open();

        button = findViewById(R.id.send);
        button.setOnClickListener(this);

        editText = findViewById(R.id.sendview);
        textView = findViewById(R.id.receview);

        thread = new Thread() {
            @Override
            public void run() {
                while (!isInterrupted()) {
                    try {
//                        Log.d(TAG, "thread running...");
                        byte[] data = new byte[10];
                        int size = serialPort.read(data, 0, data.length);
                        Log.d(TAG, Arrays.toString(data));
                        Thread.sleep(1);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        };
        thread.start();
    }

    @Override
    protected void onDestroy() {
        thread.interrupt();
        serialPort.close();
        super.onDestroy();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.send: {
                try {
                    serialPort.write(editText.getText().toString().getBytes(), 0, 1);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                break;
            }
        }
    }
}
