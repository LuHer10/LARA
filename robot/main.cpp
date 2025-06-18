#include "roboclaw.h"
#include <iostream>
#include <unistd.h>

#define SERIAL_PORT "/dev/ttyAMA0"
#define BAUD_RATE B38400

int main() {
    
    RoboClaw rcl(SERIAL_PORT, BAUD_RATE);

    const uint32_t MAX_SPEED = 2000; // max speed for M1/M2 in Signed Duty Cycle mode

    uint32_t encFL, encFR;
    uint32_t encBL, encBR;

    rcl.resetEncoders(FRONT_ADDRESS);
    rcl.resetEncoders(BACK_ADDRESS);
    
    rcl.readEncoders(FRONT_ADDRESS, encFL, encFR);
    rcl.readEncoders(BACK_ADDRESS, encBL, encBR);
    std::cout << "Encoder start values:\nencFL: " << encFL
        << " , encFR: " << encFR << "\n";
    std::cout << "encBL: " << encBL << ", encBR: " << encBR << "\n";

    rcl.M1Speed(FRONT_ADDRESS, MAX_SPEED);
    rcl.M2Speed(FRONT_ADDRESS, MAX_SPEED);
    
    rcl.M1Speed(BACK_ADDRESS, MAX_SPEED);
    rcl.M2Speed(BACK_ADDRESS, MAX_SPEED);
    
    sleep(2); // Run motors for 2 seconds

    // Stop motors (speed = 0)
    rcl.M1Speed(FRONT_ADDRESS, 0);
    rcl.M2Speed(FRONT_ADDRESS, 0);
    
    rcl.M1Speed(BACK_ADDRESS, 0);
    rcl.M2Speed(BACK_ADDRESS, 0);

    rcl.readEncoders(FRONT_ADDRESS, encFL, encFR);
    rcl.readEncoders(BACK_ADDRESS, encBL, encBR);
    std::cout << "Encoder end values:\nencFL: " << encFL
        << " , encFR: " << encFR << "\n";
    std::cout << "encBL: " << encBL << ", encBR: " << encBR << "\n";
    
    std::cout << "Motors ran for 2 seconds at half speed, now stopped." << std::endl;
    return 0;
}

