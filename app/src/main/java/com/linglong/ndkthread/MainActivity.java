package com.linglong.ndkthread;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = "KuiTag";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

    }

    public void  click(View view){
        switch (view.getId()){
            case R.id.btn1:
                NdkData data = new NdkData();
                data.name = "name";
                data.value = 100;
                Log.i(TAG,"开始:"+data.hashCode());
                NdkTool.testCallBack(data, new NdkCallBack(){
                    @Override
                    public void call(String str, NdkData data2) {
                        Log.i(TAG,"str:"+str+"--data2:"+data2.getName()+"--hash:"+data2.hashCode());
                    }
                });
                break;
            case R.id.btn2:
                NdkTool.test2();
                break;
            case R.id.btn3:
                NdkData data2 = new NdkData();
                data2.name = "test3";
                data2.value = 100;
                NdkTool.test3(data2, new NdkCallBack() {
                    @Override
                    public void call(String str, NdkData data) {
                        Log.i(TAG,"str:"+str+"--data:"+data.toString()+"--Thread:"+Thread.currentThread().getName());
                    }
                });
                break;
        }
    }

}
