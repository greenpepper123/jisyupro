#pragma once
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>
#include <array>

#define SERIAL_PORT "/dev/ttyACM0"

extern std::vector<std::array<double, 2>> points;

class Device {
public:
    int hoge;
    unsigned char buf[255];             // バッファ
    int fd;                             // ファイルディスクリプタ
    struct termios tio;                 // シリアル通信設定
    int baudRate = B2000000;
    int i;
    int len;
    int ret;
    int size;

    Device();
    void init();
    ssize_t read();
    void sendvel(float, float);
};