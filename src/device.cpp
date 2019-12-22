#include "device.hpp"

using namespace std;

Device::Device() {
    hoge = 0;
}

void Device::init() {
    fd = open(SERIAL_PORT, O_RDWR);     // デバイスをオープンする
    if (fd < 0) {
        cout<<"open error\n";
        return;
    }

    tio.c_cflag += CREAD;               // 受信有効
    tio.c_cflag += CLOCAL;              // ローカルライン（モデム制御なし）
    tio.c_cflag += CS8;                 // データビット:8bit
    tio.c_cflag += 0;                   // ストップビット:1bit
    tio.c_cflag += 0;                   // パリティ:None

    cfsetispeed( &tio, baudRate );
    cfsetospeed( &tio, baudRate );

    cfmakeraw(&tio);                    // RAWモード

    tcsetattr( fd, TCSANOW, &tio );     // デバイスに設定を行う

    ioctl(fd, TCSETS, &tio);            // ポートの設定を有効にする
}

ssize_t Device::read() {
    return ::read(fd, buf, sizeof(buf));
}

char clampr(float num) {
    if (num >= 127) {
        return 127;
    } else if (num <= 0) {
        return 0;
    } else {
        return char(num);
    }
}

char clampl(float num) {
    if (num <= -127) {
        return -127;
    } else if (num >= -1) {
        return -1;
    } else {
        return char(num);
    }
}

void Device::sendvel(float rvel, float lvel) {
    char twobyte[2] = {clampr(64+64*rvel), clampl(-64-64*lvel)};
    ::write(fd, twobyte, 2);
}