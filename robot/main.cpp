#include "roboclaw.h"
#include <iostream>
#include <unistd.h>

#define SERIAL_PORT "/dev/ttyAMA0"
#define BAUD_RATE B115200

int main() {
    
    RoboClaw rcl(SERIAL_PORT, BAUD_RATE);

    const uint32_t MAX_SPEED = 2000; // max speed for M1/M2 in Signed Duty Cycle mode

    uint32_t enc1, enc2;

    rcl.readEncoders(FRONT_ADDRESS, enc1, enc2);
    std::cout << "Encoder start values: enc1: " << enc1
        << " , enc2: " << enc2 << "\n";

    rcl.M1Speed(FRONT_ADDRESS, MAX_SPEED);
    //rcl.M2Speed(FRONT_ADDRESS, MAX_SPEED);
    
    //rcl.M1Speed(BACK_ADDRESS, MAX_SPEED);
    //rcl.M2Speed(BACK_ADDRESS, MAX_SPEED);
    sleep(2); // Run motors for 2 seconds

    // Stop motors (speed = 0)
    
    rcl.M1Speed(FRONT_ADDRESS, 0);
    //rcl.M2Speed(FRONT_ADDRESS, 0);
    
    //rcl.M1Speed(BACK_ADDRESS, 0);
    //rcl.M2Speed(BACK_ADDRESS, 0);

    rcl.readEncoders(FRONT_ADDRESS, enc1, enc2);
    //rcl.readEncoders(FRONT_ADDRESS, enc1, enc2);
    std::cout << "Encoder end values: enc1: " << enc1
        << " , enc2: " << enc2 << "\n";

    
    std::cout << "Motors ran for 2 seconds at half speed, now stopped." << std::endl;
    return 0;
}

