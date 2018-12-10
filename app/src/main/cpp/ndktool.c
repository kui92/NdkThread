//
// Created by kui on 2018/12/6.
//

#include <jni.h>
#include "tools.h"
#include "ndktool.h"
#include <android/log.h>
#include <sys/types.h>
#include<android/bitmap.h>
#include <pthread.h>
#include<android/>

#define  LOG_TAG    "KuiTag"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)


//全局存放java虚拟机对象
JavaVM* j_vm;
//线程对象
pthread_t thread_1;
//线程run方法的声明
void* run1(void*);
void test3(JNIEnv * env, jclass mclass, jobject mobject, jobject mcallback);
//全局记录回调函数的方法id
jmethodID callBack_method;
jclass callBackClass;
//全局传入的参数
jclass objClass;
jobject jobject1;
// 互斥锁变量
pthread_mutex_t mutex;


JNIEXPORT void JNICALL Java_com_linglong_ndkthread_NdkTool_testCallBack
        (JNIEnv * env, jclass mclass, jobject mobject, jobject mcallback){
    LOGD("进入jni:");
    jclass objClass = (*env)->GetObjectClass(env,mobject);
    jclass callBack = (*env)->GetObjectClass(env,mcallback);
    jmethodID getMethod = (*env)->GetMethodID(env,objClass, "getName", "()Ljava/lang/String;");
    jmethodID setMethod = (*env)->GetMethodID(env,objClass,"setName","(Ljava/lang/String;)V");
    //jmethodID initId = (*env)->GetMethodID(env,objClass,"<init>","(Ljava/lang/String;)V");
    jmethodID callId = (*env)->GetMethodID(env,callBack,"call","(Ljava/lang/String;Lcom/linglong/ndkthread/NdkData;)V");
    jstring str = (*env)->CallObjectMethod(env,mobject, getMethod);
    char * getStr = jstringTostring(env,str);
    LOGD("11111:%s",getStr);
    const char * setStr = strAdd("jni_",getStr);
    LOGD("2222:");
    jstring setJstring = charTojstring(env,setStr);
    //jobject newObj = (*env)->NewObject(env,objClass,initId,setJstring);
    (*env)->CallVoidMethod(env,mobject,setMethod,setJstring);
    jstring s = charTojstring(env,"jni_callback");
    (*env)->CallVoidMethod(env,mcallback,callId,s,mobject);
}

void test2(JNIEnv *env,jclass cls){
    LOGD("test2");
}

void test3(JNIEnv * env, jclass mclass, jobject mobject, jobject mcallback){
    LOGD("test3");
    objClass = (*env)->GetObjectClass(env,mobject);
    callBackClass = (*env)->GetObjectClass(env,mcallback);
    jobject1 = (*env)->NewGlobalRef(env,mobject);
    jobject obj = (*env)->NewGlobalRef(env,mcallback);
    callBack_method = (*env)->GetMethodID(env,callBackClass,"call","(Ljava/lang/String;Lcom/linglong/ndkthread/NdkData;)V");
    pthread_create(&thread_1,NULL,run1,obj);
}

/***************************************************/
//这里设置java中全类名
static const char *classPathName = "com/linglong/ndkthread/NdkTool";
//设置java中对应类名中的各种方法的各种属性，以便查找到：
//这里只有一个方法，所以数组中就装了一个元素:
//1.字符串类型：java中的方法名
//2.字符串类型：signature 用来表示方法的参数和返回值类型，在.h的自动生次文档的注释中可找到
//3.void*类型：c中方法名(也就是上面的方法名)
static JNINativeMethod methods[] = { { "test2", "()V", (void*) test2 },
                                      { "test3", "(Lcom/linglong/ndkthread/NdkData;Lcom/linglong/ndkthread/NdkCallBack;)V", (void*) test3 } };

static int registerNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods) {
    jclass clazz = (*env)->FindClass(env,className);
    (*env)->RegisterNatives(env,clazz, gMethods, numMethods);
    return JNI_TRUE;
}
static int registerNatives(JNIEnv* env) {
    return registerNativeMethods(env, classPathName, methods,sizeof(methods) / sizeof(methods[0]));
}



//这里重写onload方法，这样java中调用"System.loadLibrary("")"的时候就会调用这个onload方法
//默认情况下调用的是默认的onload方法，
//要使自己的onload方法被调用到，需要按以下的格式书写方法，其中"JNIEXPORT"可要可不要
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGD("JNI_OnLoad");
    //首先是将vm指针记录在全局变量中，以便线程中要用到
    j_vm = vm;
    JNIEnv *env;
    //分配一个JNIEnv*，存放在env中，之后调用方法的时候要用到
    if ((*vm)->GetEnv(vm,(void **) &env, JNI_VERSION_1_4) != JNI_OK)
        return -1;
    //通过调用上面的方法，相当于在这个JNIEnv中画了一张对照表，java中要执行什么方法，就到上面去找，找到了后再去调用c中对应的方法
    if (registerNatives(env) != JNI_TRUE) {
        return -1;
    }
    return JNI_VERSION_1_4;
}


//线程的run方法
void *run1(void* args) {
    LOGD("run1");
    //记录从jvm中申请JNIEnv*的状态
    int status;
    //用于存放申请过来的JNIEnv*
    JNIEnv *env;
    //用于标记线程的状态，用于开启，释放
    jboolean isAttached = JNI_FALSE;
    //获取当前状态，查看是否已经拥有过JNIEnv指针
    status = (*j_vm)->GetEnv(j_vm,(void**) &env, JNI_VERSION_1_4);
    if (status < 0) {
        //将当前线程依附于java虚拟机：
        //这样能够得到一个JNIEnv*指针，
        //该线程也能够像java线程一样，在一定规则下运行
        //这个状态将持续直到调用detachCurrentThread方法
        status = (*j_vm)->AttachCurrentThread(j_vm,&env, NULL);
        if (status < 0)
            return NULL;
        isAttached = JNI_TRUE;
    }
    char * ret = "Ndk Thread";
    jstring jstring1 = charTojstring(env,ret);
    //执行这个线程要做的事情
    (*env)->CallVoidMethod(env,(jobject) args, callBack_method,jstring1,jobject1);
    //执行完了释放
    if (isAttached)
    (*j_vm)->DetachCurrentThread(j_vm);
    return NULL;
}

/**
** 关于线程锁 https://www.jianshu.com/p/c199c2bbe83e
 **/