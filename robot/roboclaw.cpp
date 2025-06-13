#include "roboclaw.h"

void RoboClaw::M1Duty(uint8_t address, uint16_t duty)
{
    uint8_t packet[6];
    packet[0] = address;
    packet[1] = 32;
    packet[2] = (duty>>8) & 0xFF;
    packet[3] = duty & 0xFF;

    uint8_t crc = crc16(packet, 4);
    packet[4] = (crc>>8) & 0xFF;
    packet[5] = crc & 0xFF;

    write(fd, packet, 6);
    tcdrain(fd);
}

void RoboClaw::M2Duty(uint8_t address, uint16_t duty)
{
    uint8_t packet[6];
    packet[0] = address;
    packet[1] = 3;
    packet[2] = (duty>>8) & 0xFF;
    packet[3] = duty & 0xFF;

    uint8_t crc = crc16(packet, 4);
    packet[4] = (crc>>8) & 0xFF;
    packet[5] = crc & 0xFF;

    write(fd, packet, 6);
    tcdrain(fd);
}
