#include "roboclaw.h"

void RoboClaw::M1Duty(uint8_t address, uint16_t duty)
{
    uint8_t packet[6];
    packet[0] = address;
    packet[1] = M1_DUTY; // Command for M1 duty cycle
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
    packet[1] = M2_DUTY; // Command for M2 duty cycle
    packet[2] = (duty>>8) & 0xFF;
    packet[3] = duty & 0xFF;

    uint8_t crc = crc16(packet, 4);
    packet[4] = (crc>>8) & 0xFF;
    packet[5] = crc & 0xFF;

    write(fd, packet, 6);
    tcdrain(fd);
}

void RoboClaw::M1M2Duty(uint8_t address, uint16_t duty1, uint16_t duty2)
{
    uint8_t packet[8];
    packet[0] = address;
    packet[1] = M1_M2_DUTY; // Command for M1 and M2 duty cycle
    packet[2] = (duty1>>8) & 0xFF;
    packet[3] = duty1 & 0xFF;
    packet[4] = (duty2>>8) & 0xFF;
    packet[5] = duty2 & 0xFF;

    uint16_t crc = crc16(packet, 6);
    packet[6] = (crc>>8) & 0xFF;
    packet[7] = crc & 0xFF;

    write(fd, packet, 8);
    tcdrain(fd);
}

void RoboClaw::M1Speed(uint8_t address, uint32_t speed)
{
    uint8_t packet[8];
    packet[0] = address;
    packet[1] = M1_SPEED; // Command for M1 speed
    packet[2] = (speed>>24) & 0xFF;
    packet[3] = (speed>>16) & 0xFF;
    packet[4] = (speed>>8) & 0xFF;
    packet[5] = speed & 0xFF;

    uint16_t crc = crc16(packet, 6);
    packet[6] = (crc>>8) & 0xFF;
    packet[7] = crc & 0xFF;

    write(fd, packet, 8);
    tcdrain(fd);
}

void RoboClaw::M2Speed(uint8_t address, uint32_t speed)
{
    uint8_t packet[8];
    packet[0] = address;
    packet[1] = M2_SPEED; // Command for M2 speed
    packet[2] = (speed>>24) & 0xFF;
    packet[3] = (speed>>16) & 0xFF;
    packet[4] = (speed>>8) & 0xFF;
    packet[5] = speed & 0xFF;

    uint16_t crc = crc16(packet, 6);
    packet[6] = (crc>>8) & 0xFF;
    packet[7] = crc & 0xFF;

    write(fd, packet, 8);
    tcdrain(fd);
}

void RoboClaw::M1M2Speed(uint8_t address, uint32_t speed1, uint32_t speed2)
{
    uint8_t packet[12];
    packet[0] = address;
    packet[1] = M1_M2_SPEED; // Command for M1 and M2 speed
    packet[2] = (speed1>>24) & 0xFF;
    packet[3] = (speed1>>16) & 0xFF;
    packet[4] = (speed1>>8) & 0xFF;
    packet[5] = speed1 & 0xFF;
    packet[6] = (speed2>>24) & 0xFF;
    packet[7] = (speed2>>16) & 0xFF;
    packet[8] = (speed2>>8) & 0xFF;
    packet[9] = speed2 & 0xFF;

    uint16_t crc = crc16(packet, 10);
    packet[10] = (crc>>8) & 0xFF;
    packet[11] = crc & 0xFF;

    write(fd, packet, 12);
    tcdrain(fd);
}
