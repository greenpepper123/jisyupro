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
#include <cmath>

#define SERIAL_PORT "/dev/ttyACM0"

constexpr float chasis_radius = 0.05;
constexpr float wheel_radius = 0.055;
constexpr float max_omega = M_PI/3.0/0.25;

extern std::vector<std::array<float, 2>> points;

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

char clampchar(float);