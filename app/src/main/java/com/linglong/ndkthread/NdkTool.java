package com.linglong.ndkthread;

/**
 * @author : kui
 * date   : 2018/12/6  13:57
 * desc   :
 * version: 1.0
 */
public class NdkTool {

    static {
        System.loadLibrary("kui");
    }

    public static native void testCallBack(NdkData data,NdkCallBack callBack);

    public static native void test2();

    public static native void test3(NdkData data,NdkCallBack callBack);

}
