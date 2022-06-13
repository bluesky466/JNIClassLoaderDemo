package me.linjw.demo.jni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import me.linjw.demo.jni.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'jni' library on application startup.
    static {
        System.loadLibrary("jni");
    }

    public static int DATA_IN_JAVA = 123;

    private ActivityMainBinding binding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        new Thread(new Runnable() {
            @Override
            public void run() {
                test();
            }
        }).start();
    }

    /**
     * A native method that is implemented by the 'jni' native library,
     * which is packaged with this application.
     */
    public native void test();
}