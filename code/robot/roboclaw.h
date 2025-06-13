#ifndef ROBOCLAW_H
#define ROBOCLAW_H

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>

#define FRONT_ADDRESS 0x80
#define BACK_ADDRESS  0x81
#define DEFAULT_PORT "/dev/ttyAMA0"
#define DEFAULT_BAUDS B1152000

class RoboClaw
{
private:
    int baudRate;
    const char* serialPort;

    int fd;

    uint16_t crc16(const uint8_t* data, size_t length) {
        uint16_t crc = 0;
        for(size_t i = 0; i < length; ++i) {
            crc ^= ((uint16_t)data[i]) << 8;
            for(int j = 0; j < 8; ++j) {
                if(crc & 0x8000)
                    crc = (crc << 1) ^ 0x1021;
                else
                    crc <<= 1;
            }
        }
        return crc;
    }


public:
    RoboClaw(const char* _serialPort =  DEFAULT_PORT, int _baudRate = DEFAULT_BAUDS):serialPort(_serialPort),baudRate(_baudRate)
    {
        int fd = open(serialPort, O_RDWR | O_NOCTTY);
        if (fd == -1)
            perror("openSerialPort: Unable to open port");

        struct termios options{};
        tcgetattr(fd, &options);
        cfsetispeed(&options, baudRate);
        cfsetospeed(&options, baudRate);
        options.c_cflag |= (CLOCAL | CREAD);
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CRTSCTS;
        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        options.c_iflag &= ~(IXON | IXOFF | IXANY);
        options.c_oflag &= ~OPOST;
        tcsetattr(fd, TCSANOW, &options);

    }

    ~RoboClaw(){close(fd);}

    void M1Duty(uint8_t address, uint16_t duty);
    void M2Duty(uint8_t address, uint16_t duty);
};

#endif //ROBOCLAW_H