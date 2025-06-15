#include "roboclaw.h"
#include <iostream>
#include <unistd.h>

#define SERIAL_PORT "/dev/ttyAMA0"
#define BAUD_RATE B115200

int main() {
    
    RoboClaw rcl(SERIAL_PORT, BAUD_RATE);

    const uint16_t MAX_SPEED = 32767; // max speed for M1/M2 in Signed Duty Cycle mode

    rcl.M1Duty(FRONT_ADDRESS, MAX_SPEED);
    rcl.M2Duty(FRONT_ADDRESS, MAX_SPEED);
    sleep(2); // Run motors for 2 seconds

    // Stop motors (speed = 0)
    rcl.M1Duty(FRONT_ADDRESS, 0);
    rcl.M2Duty(FRONT_ADDRESS, 0);

    
    std::cout << "Motors ran for 2 seconds at half speed, now stopped." << std::endl;
    return 0;
}

