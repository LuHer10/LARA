#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

// Adjust these as needed
#define DEVICE_ADDRESS 0x80
#define SERIAL_PORT "/dev/ttyAMA0"

#define BAUD_RATE B115200

// CRC computation for packet serial
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

void sendMotorCommand(int fd, uint8_t address, uint8_t command, uint32_t speed) {
    uint8_t packet[9];
    packet[0] = address;
    packet[1] = command;
    packet[2] = (speed >> 24) & 0xFF;
    packet[3] = (speed >> 16) & 0xFF;
    packet[4] = (speed >> 8) & 0xFF;
    packet[5] = speed & 0xFF;

    uint16_t crc = crc16(packet, 6);
    packet[6] = (crc >> 8) & 0xFF;
    packet[7] = crc & 0xFF;

    write(fd, packet, 8);
    tcdrain(fd);
}

int openSerialPort(const char* device) {
    int fd = open(device, O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("openSerialPort: Unable to open port");
        return -1;
    }

    struct termios options{};
    tcgetattr(fd, &options);
    cfsetispeed(&options, BAUD_RATE);
    cfsetospeed(&options, BAUD_RATE);
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

    return fd;
}

int main() {
    int fd = openSerialPort(SERIAL_PORT);
    if (fd == -1) return 1;

    const uint32_t MAX_SPEED = 0x7FFF; // max speed for M1/M2 in Signed Duty Cycle mode
    const uint32_t HALF_SPEED = MAX_SPEED / 2;

    // Command 35 = Drive M1 at Signed Duty Cycle
    // Command 36 = Drive M2 at Signed Duty Cycle

    sendMotorCommand(fd, DEVICE_ADDRESS, 35, HALF_SPEED);
    sleep(2); // Run motors for 2 seconds

    // Stop motors (speed = 0)
    sendMotorCommand(fd, DEVICE_ADDRESS, 35, 0);

    close(fd);
    std::cout << "Motors ran for 2 seconds at half speed, now stopped." << std::endl;
    return 0;
}

