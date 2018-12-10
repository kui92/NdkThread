package com.linglong.ndkthread;

/**
 * @author : kui
 * date   : 2018/12/6  11:20
 * desc   :
 * version: 1.0
 */
public class NdkData {

    public String name;

    public int value;


    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getValue() {
        return value;
    }

    public void setValue(int value) {
        this.value = value;
    }

    @Override
    public String toString() {
        return "NdkData{" +
                "name='" + name + '\'' +
                ", value=" + value +
                '}';
    }
}
