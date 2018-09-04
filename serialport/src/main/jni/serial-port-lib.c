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

static struct termios termios_p;

JNIEXPORT jobject JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1open(JNIEnv *env, jobject instance,
                                                            jstring path_) {

    int fd;

    const char *path = (*env)->GetStringUTFChars(env, path_, 0);
    LOGD("file path: %s", path);
    fd = open(path, O_RDWR);
    LOGD("fd: %d", fd);
    if (-1 == fd) {
        LOGE("open failed");
        return NULL;
    }
    (*env)->ReleaseStringUTFChars(env, path_, path);

    memset(&termios_p, 0, sizeof(struct termios));
    if (-1 == tcgetattr(fd, &termios_p)) {
        LOGE("tcgetattr failed");
        close(fd);
        return NULL;
    }

    cfmakeraw(&termios_p);

    if (-1 == tcsetattr(fd, TCSANOW, &termios_p)) {
        LOGE("tcsetattr failed");
        close(fd);
        return NULL;
    }

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jmethodID FileDescriptorMethodID = (*env)->GetMethodID(env, FileDescriptorClass, "<init>", "()V");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jobject fileDescriptor = (*env)->NewObject(env, FileDescriptorClass, FileDescriptorMethodID);
    (*env)->SetIntField(env, fileDescriptor, descriptorFieldID, (jint) fd);

    return fileDescriptor;
}

JNIEXPORT void JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1close(JNIEnv *env, jobject instance,
                                                             jobject fd) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);
    LOGD("fd: %d", descriptor);

    if (-1 == close(descriptor)) {
        LOGE("close failed");
        return;
    }
}

JNIEXPORT void JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1baudrate(JNIEnv *env, jobject instance,
                                                                     jobject fd, jint baudRate) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);
    LOGD("fd: %d", descriptor);

//    struct termios termios_p;
//    memset(&termios_p, 0, sizeof(struct termios));
//    if (-1 == tcgetattr(descriptor, &termios_p)) {
//        LOGE("tcgetattr failed");
//        return;
//    }

    speed_t speed;

    switch (baudRate) {
        case 0:       { speed = B0;       LOGD("baudRate: %d", baudRate); break; }
        case 50:      { speed = B50;      LOGD("baudRate: %d", baudRate); break; }
        case 75:      { speed = B75;      LOGD("baudRate: %d", baudRate); break; }
        case 110:     { speed = B110;     LOGD("baudRate: %d", baudRate); break; }
        case 134:     { speed = B134;     LOGD("baudRate: %d", baudRate); break; }
        case 150:     { speed = B150;     LOGD("baudRate: %d", baudRate); break; }
        case 200:     { speed = B200;     LOGD("baudRate: %d", baudRate); break; }
        case 300:     { speed = B300;     LOGD("baudRate: %d", baudRate); break; }
        case 600:     { speed = B600;     LOGD("baudRate: %d", baudRate); break; }
        case 1200:    { speed = B1200;    LOGD("baudRate: %d", baudRate); break; }
        case 1800:    { speed = B1800;    LOGD("baudRate: %d", baudRate); break; }
        case 2400:    { speed = B2400;    LOGD("baudRate: %d", baudRate); break; }
        case 4800:    { speed = B4800;    LOGD("baudRate: %d", baudRate); break; }
        case 9600:    { speed = B9600;    LOGD("baudRate: %d", baudRate); break; }
        case 19200:   { speed = B19200;   LOGD("baudRate: %d", baudRate); break; }
        case 38400:   { speed = B38400;   LOGD("baudRate: %d", baudRate); break; }
        case 57600:   { speed = B57600;   LOGD("baudRate: %d", baudRate); break; }
        case 115200:  { speed = B115200;  LOGD("baudRate: %d", baudRate); break; }
        case 230400:  { speed = B230400;  LOGD("baudRate: %d", baudRate); break; }
        case 460800:  { speed = B460800;  LOGD("baudRate: %d", baudRate); break; }
        case 500000:  { speed = B500000;  LOGD("baudRate: %d", baudRate); break; }
        case 576000:  { speed = B576000;  LOGD("baudRate: %d", baudRate); break; }
        case 921600:  { speed = B921600;  LOGD("baudRate: %d", baudRate); break; }
        case 1000000: { speed = B1000000; LOGD("baudRate: %d", baudRate); break; }
        case 1152000: { speed = B1152000; LOGD("baudRate: %d", baudRate); break; }
        case 1500000: { speed = B1500000; LOGD("baudRate: %d", baudRate); break; }
        case 2000000: { speed = B2000000; LOGD("baudRate: %d", baudRate); break; }
        case 2500000: { speed = B2500000; LOGD("baudRate: %d", baudRate); break; }
        case 3000000: { speed = B3000000; LOGD("baudRate: %d", baudRate); break; }
        case 3500000: { speed = B3500000; LOGD("baudRate: %d", baudRate); break; }
        case 4000000: { speed = B4000000; LOGD("baudRate: %d", baudRate); break; }
        default:      { LOGE("baudRate: invalid parameter"); return; }
    }

    if (-1 == cfsetspeed(&termios_p, speed)) {
        LOGE("cfsetspeed failed");
        return;
    }
}

JNIEXPORT void JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1databits(JNIEnv *env, jobject instance,
                                                                     jobject fd, jint dataBits) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);
    LOGD("fd: %d", descriptor);

//    struct termios termios_p;
//    memset(&termios_p, 0, sizeof(struct termios));
//    if (-1 == tcgetattr(descriptor, &termios_p)) {
//        LOGE("tcgetattr failed");
//        return;
//    }

    termios_p.c_cflag &= ~CSIZE;

    switch (dataBits) {
        case DATABITS_5: {
            termios_p.c_cflag |= CS5;
            LOGD("dataBits: %d", dataBits);
            break;
        }
        case DATABITS_6: {
            termios_p.c_cflag |= CS6;
            LOGD("dataBits: %d", dataBits);
            break;
        }
        case DATABITS_7: {
            termios_p.c_cflag |= CS7;
            LOGD("dataBits: %d", dataBits);
            break;
        }
        case DATABITS_8: {
            termios_p.c_cflag |= CS8;
            LOGD("dataBits: %d", dataBits);
            break;
        }
        default: {
            LOGE("dataBits: invalid parameter");
            return;
        }
    }

    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
        LOGE("tcsetattr failed");
        return;
    }
}

JNIEXPORT void JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1parity(JNIEnv *env, jobject instance,
                                                                   jobject fd, jint parity) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);
    LOGD("fd: %d", descriptor);

//    struct termios termios_p;
//    memset(&termios_p, 0, sizeof(struct termios));
//    if (-1 == tcgetattr(descriptor, &termios_p)) {
//        LOGE("tcgetattr failed");
//        return;
//    }

    switch (parity) {
        case PARITY_NONE: {
            termios_p.c_cflag &= ~PARENB;
            LOGD("parity: None");
            break;
        }
        case PARITY_ODD: {
            termios_p.c_cflag |= PARENB;
            termios_p.c_cflag &= ~CMSPAR;
            termios_p.c_cflag |= PARODD;
            LOGD("parity: Odd");
            break;
        }
        case PARITY_EVEN: {
            termios_p.c_cflag |= PARENB;
            termios_p.c_cflag &= ~CMSPAR;
            termios_p.c_cflag &= ~PARODD;
            LOGD("parity: Even");
            break;
        }
        case PARITY_MARK: {
            termios_p.c_cflag |= PARENB;
            termios_p.c_cflag |= CMSPAR;
            termios_p.c_cflag |= PARODD;
            LOGD("parity: Mark");
            break;
        }
        case PARITY_SPACE: {
            termios_p.c_cflag |= PARENB;
            termios_p.c_cflag |= CMSPAR;
            termios_p.c_cflag &= ~PARODD;
            LOGD("parity: Space");
            break;
        }
        default: {
            LOGE("parity: invalid parameter");
            return;
        }
    }

    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
        LOGE("tcsetattr failed");
        return;
    }
}

JNIEXPORT void JNICALL
Java_com_redleaves_serialport_SerialPort_serial_1port_1set_1stopbits(JNIEnv *env, jobject instance,
                                                                     jobject fd, jint stopBits) {

    jclass FileDescriptorClass = (*env)->FindClass(env, "java/io/FileDescriptor");
    jfieldID descriptorFieldID = (*env)->GetFieldID(env, FileDescriptorClass, "descriptor", "I");
    jint descriptor = (*env)->GetIntField(env, fd, descriptorFieldID);
    LOGD("fd: %d", descriptor);

//    struct termios termios_p;
//    memset(&termios_p, 0, sizeof(struct termios));
//    if (-1 == tcgetattr(descriptor, &termios_p)) {
//        LOGE("tcgetattr failed");
//        return;
//    }

    switch (stopBits) {
        case STOPBITS_1: {
            termios_p.c_cflag &= ~CSTOPB;
            LOGD("stopBits: %d", stopBits);
            break;
        }
        case STOPBITS_2: {
            termios_p.c_cflag |= CSTOPB;
            LOGD("stopBits: %d", stopBits);
            break;
        }
        default: {
            LOGE("stopBits: invalid parameter");
            return;
        }
    }

    if (-1 == tcsetattr(descriptor, TCSANOW, &termios_p)) {
        LOGE("tcsetattr failed");
        return;
    }
}