#include <fcntl.h>
#include <termios.h>
#define STDIN_FILENO 0

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "dynamixel_sdk.h"  // Uses DYNAMIXEL SDK library

#define ADDR_TORQUE_ENABLE          64
#define ADDR_GOAL_POSITION          116
#define ADDR_PRESENT_POSITION       132
#define MINIMUM_POSITION_LIMIT      0  // Refer to the Minimum Position Limit of product eManual
#define MAXIMUM_POSITION_LIMIT      4095  // Refer to the Maximum Position Limit of product eManual
#define BAUDRATE                    57600

#define PROTOCOL_VERSION  2.0

#define DXL_ID  1

#define DEVICENAME  "/dev/ttyUSB0"

#define TORQUE_ENABLE                   1
#define TORQUE_DISABLE                  0
#define DXL_MOVING_STATUS_THRESHOLD     20  // DYNAMIXEL moving status threshold
#define ESC_ASCII_VALUE                 0x1b


class Servo
{
private:
    dynamixel::PortHandler *portHandler;
    dynamixel::PacketHandler *packetHandler;

    bool initialized = false;

    int dxl_comm_result = COMM_TX_FAIL;
    uint8_t dxl_error = 0;

    int32_t goalPos[253];
    int32_t currPos[253];

public:
    int init();
    int init(const char *devName);
    void enableTorque(int id);
    void disableTorque(int id);
    void writePos(int id, int32_t pos);
    int32_t readPos(int id);
};