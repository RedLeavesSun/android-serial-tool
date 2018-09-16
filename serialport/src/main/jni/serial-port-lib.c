#include <jni.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#include "android/log.h"

static const char *TAG = "serial-port";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

#define DATABITS_5   5
#define DATABITS_6   6
#define DATABITS_7   7
#define DATABITS_8   8

#define PARITY_NONE  0
#define PARITY_ODD   1
#define PARITY_EVEN  2
#define PARITY_MARK  3
#define PARITY_SPACE 4

#define STOPBITS_1   1
#define STOPBITS_2   2

JNIEXPORT jobject JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1open(JNIEnv *env, jobject instance,
                                                            jstring path_) {

    int descriptor;

    const char *path = (*env)->GetStringUTFChars(env, path_, 0);
    LOGD("file path: %s", path);
    descriptor = open(path, O_RDWR);
    if (-1 == descriptor) {
        LOGE("fd: %d, open failed", descriptor);
        return NULL;
    }
    (*env)->ReleaseStringUTFChars(env, path_, path);

    struct termios termios_p;
    memset(&termios_p, 0, sizeof(struct termios));
    if (-1 == tcgetattr(descriptor, &termios_p)) {
        LOGE("fd: %d, tcgetattr failed", descriptor);
        close(descriptor);
        return NULL;
    }

    cfmakeraw(&termios_p);

    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
        LOGE("fd: %d, tcsetattr failed", descriptor);
        close(descriptor);
        return NULL;
    }

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jmethodID FileDescriptorMethodID = (*env)->GetMethodID(env, FileDescriptorClass, "<init>", "()V");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jobject fileDescriptor = (*env)->NewObject(env, FileDescriptorClass, FileDescriptorMethodID);
    (*env)->SetIntField(env, fileDescriptor, descriptorFieldID, (jint) descriptor);

    return fileDescriptor;
}

JNIEXPORT jboolean JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1close(JNIEnv *env, jobject instance,
                                                             jobject fd) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);

    if (-1 == close(descriptor)) {
        LOGE("fd: %d, close failed", descriptor);
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1baudrate(JNIEnv *env, jobject instance,
                                                                     jobject fd, jint baudRate) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);

    struct termios termios_p;
    memset(&termios_p, 0, sizeof(struct termios));
    if (-1 == tcgetattr(descriptor, &termios_p)) {
        LOGE("fd: %d, tcgetattr failed", descriptor);
        return;
    }

    speed_t speed;

    switch (baudRate) {
        case 0:       { speed = B0;       LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 50:      { speed = B50;      LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 75:      { speed = B75;      LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 110:     { speed = B110;     LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 134:     { speed = B134;     LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 150:     { speed = B150;     LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 200:     { speed = B200;     LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 300:     { speed = B300;     LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 600:     { speed = B600;     LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 1200:    { speed = B1200;    LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 1800:    { speed = B1800;    LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 2400:    { speed = B2400;    LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 4800:    { speed = B4800;    LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 9600:    { speed = B9600;    LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 19200:   { speed = B19200;   LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 38400:   { speed = B38400;   LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 57600:   { speed = B57600;   LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 115200:  { speed = B115200;  LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 230400:  { speed = B230400;  LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 460800:  { speed = B460800;  LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 500000:  { speed = B500000;  LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 576000:  { speed = B576000;  LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 921600:  { speed = B921600;  LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 1000000: { speed = B1000000; LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 1152000: { speed = B1152000; LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 1500000: { speed = B1500000; LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 2000000: { speed = B2000000; LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 2500000: { speed = B2500000; LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 3000000: { speed = B3000000; LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 3500000: { speed = B3500000; LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        case 4000000: { speed = B4000000; LOGD("fd: %d, baudRate: %d", descriptor, baudRate); break; }
        default:      { LOGE("baudRate: invalid parameter"); return; }
    }

    if (-1 == cfsetspeed(&termios_p, speed)) {
        LOGE("fd: %d, cfsetspeed failed", descriptor);
        return;
    }

    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
        LOGE("fd: %d, tcsetattr failed", descriptor);
        return;
    }
}

JNIEXPORT void JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1databits(JNIEnv *env, jobject instance,
                                                                     jobject fd, jint dataBits) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);

    struct termios termios_p;
    memset(&termios_p, 0, sizeof(struct termios));
    if (-1 == tcgetattr(descriptor, &termios_p)) {
        LOGE("fd: %d, tcgetattr failed", descriptor);
        return;
    }

    termios_p.c_cflag &= ~CSIZE;

    switch (dataBits) {
        case DATABITS_5: {
            termios_p.c_cflag |= CS5;
            LOGD("fd: %d, dataBits: %d", descriptor, dataBits);
            break;
        }
        case DATABITS_6: {
            termios_p.c_cflag |= CS6;
            LOGD("fd: %d, dataBits: %d", descriptor, dataBits);
            break;
        }
        case DATABITS_7: {
            termios_p.c_cflag |= CS7;
            LOGD("fd: %d, dataBits: %d", descriptor, dataBits);
            break;
        }
        case DATABITS_8: {
            termios_p.c_cflag |= CS8;
            LOGD("fd: %d, dataBits: %d", descriptor, dataBits);
            break;
        }
        default: {
            LOGE("fd: %d, dataBits: invalid parameter", descriptor);
            return;
        }
    }

    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
        LOGE("fd: %d, tcsetattr failed", descriptor);
        return;
    }
}

JNIEXPORT void JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1parity(JNIEnv *env, jobject instance,
                                                                   jobject fd, jint parity) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);

    struct termios termios_p;
    memset(&termios_p, 0, sizeof(struct termios));
    if (-1 == tcgetattr(descriptor, &termios_p)) {
        LOGE("fd: %d, tcgetattr failed", descriptor);
        return;
    }

    switch (parity) {
        case PARITY_NONE: {
            termios_p.c_cflag &= ~PARENB;
            LOGD("fd: %d, parity: None", descriptor);
            break;
        }
        case PARITY_ODD: {
            termios_p.c_cflag |= PARENB;
            termios_p.c_cflag &= ~CMSPAR;
            termios_p.c_cflag |= PARODD;
            LOGD("fd: %d, parity: Odd", descriptor);
            break;
        }
        case PARITY_EVEN: {
            termios_p.c_cflag |= PARENB;
            termios_p.c_cflag &= ~CMSPAR;
            termios_p.c_cflag &= ~PARODD;
            LOGD("fd: %d, parity: Even", descriptor);
            break;
        }
        case PARITY_MARK: {
            termios_p.c_cflag |= PARENB;
            termios_p.c_cflag |= CMSPAR;
            termios_p.c_cflag |= PARODD;
            LOGD("fd: %d, parity: Mark", descriptor);
            break;
        }
        case PARITY_SPACE: {
            termios_p.c_cflag |= PARENB;
            termios_p.c_cflag |= CMSPAR;
            termios_p.c_cflag &= ~PARODD;
            LOGD("fd: %d, parity: Space", descriptor);
            break;
        }
        default: {
            LOGE("fd: %d, parity: invalid parameter", descriptor);
            return;
        }
    }

    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
        LOGE("fd: %d, tcsetattr failed", descriptor);
        return;
    }
}

JNIEXPORT void JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1stopbits(JNIEnv *env, jobject instance,
                                                                     jobject fd, jint stopBits) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);

    struct termios termios_p;
    memset(&termios_p, 0, sizeof(struct termios));
    if (-1 == tcgetattr(descriptor, &termios_p)) {
        LOGE("fd: %d, tcgetattr failed", descriptor);
        return;
    }

    switch (stopBits) {
        case STOPBITS_1: {
            termios_p.c_cflag &= ~CSTOPB;
            LOGD("fd: %d, stopBits: %d", descriptor, stopBits);
            break;
        }
        case STOPBITS_2: {
            termios_p.c_cflag |= CSTOPB;
            LOGD("fd: %d, stopBits: %d", descriptor, stopBits);
            break;
        }
        default: {
            LOGE("fd: %d, stopBits: invalid parameter", descriptor);
            return;
        }
    }

    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
        LOGE("fd: %d, tcsetattr failed", descriptor);
        return;
    }
}

JNIEXPORT void JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1read_1timeout(JNIEnv *env,
                                                                          jobject instance,
                                                                          jobject fd,
                                                                          jint timeout) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);

    struct termios termios_p;
    memset(&termios_p, 0, sizeof(struct termios));
    if (-1 == tcgetattr(descriptor, &termios_p)) {
        LOGE("fd: %d, tcgetattr failed", descriptor);
        return;
    }

    if (timeout <= 255) {
        LOGD("fd: %d, timeout: %d ms", descriptor, timeout * 100);
        termios_p.c_cc[VTIME] = (unsigned char)timeout;
    } else {
        LOGE("fd: %d, timeout: %d ms, is greater than 25500 ms", descriptor, timeout * 100);
        return;
    }

    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
        LOGE("fd: %d, tcsetattr failed", descriptor);
        return;
    }
}

//JNIEXPORT void JNICALL
//Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1write_1timeout(JNIEnv *env,
//                                                                           jobject instance,
//                                                                           jobject fd,
//                                                                           jint timeout) {
//
//    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
//    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
//    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);
//
//    struct termios termios_p;
//    memset(&termios_p, 0, sizeof(struct termios));
//    if (-1 == tcgetattr(descriptor, &termios_p)) {
//        LOGE("fd: %d, tcgetattr failed", descriptor);
//        return;
//    }
//
//    if (timeout <= 255) {
//        LOGD("fd: %d, timeout: %d ms", descriptor, timeout * 100);
//        termios_p.c_cc[VTIME] = timeout;
//    } else {
//        LOGE("fd: %d, timeout: %d ms, is greater than 25500 ms", descriptor, timeout * 100);
//        return;
//    }
//
//    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
//        LOGE("fd: %d, tcsetattr failed", descriptor);
//        return;
//    }
//}

JNIEXPORT void JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1read_1bytes_1threshold(JNIEnv *env,
                                                                                   jobject instance,
                                                                                   jobject fd,
                                                                                   jint readBytesThreshold) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);

    struct termios termios_p;
    memset(&termios_p, 0, sizeof(struct termios));
    if (-1 == tcgetattr(descriptor, &termios_p)) {
        LOGE("fd: %d, tcgetattr failed", descriptor);
        return;
    }

    if (readBytesThreshold <= 255) {
        LOGD("fd: %d, rece bytes threshold: %d", descriptor, readBytesThreshold);
        termios_p.c_cc[VMIN] = (unsigned char)readBytesThreshold;
    } else {
        LOGE("fd: %d, rece bytes threshold: %d, is greater than 255", descriptor, readBytesThreshold);
        return;
    }

    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
        LOGE("fd: %d, tcsetattr failed", descriptor);
        return;
    }
}

//JNIEXPORT void JNICALL
//Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1send_1bytes_1threshold(JNIEnv *env,
//                                                                                   jobject instance,
//                                                                                   jobject fd,
//                                                                                   jint sendBytesThreshold) {
//
//    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
//    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
//    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);
//
//    struct termios termios_p;
//    memset(&termios_p, 0, sizeof(struct termios));
//    if (-1 == tcgetattr(descriptor, &termios_p)) {
//        LOGE("fd: %d, tcgetattr failed", descriptor);
//        return;
//    }
//
//    if (sendBytesThreshold <= 255) {
//        LOGD("fd: %d, send bytes threshold: %d", descriptor, sendBytesThreshold);
//        termios_p.c_cc[VMIN] = (unsigned char)sendBytesThreshold;
//    } else {
//        LOGE("fd: %d, send bytes threshold: %d, is greater than 255", descriptor, sendBytesThreshold);
//        return;
//    }
//
//    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
//        LOGE("fd: %d, tcsetattr failed", descriptor);
//        return;
//    }
//}
