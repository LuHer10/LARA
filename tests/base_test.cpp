#include "../robot/base.h"
#include <iostream>
#include <unistd.h>

#define SERIAL_PORT "/dev/ttyAMA0"
#define BAUD_RATE B38400

int main() {
    
    //RoboClaw rcl(SERIAL_PORT, BAUD_RATE);
    Base base;


    const uint32_t MAX_SPEED = 2000; // max speed for M1/M2 in Signed Duty Cycle mode

    Odometry odo;
    
    base.readEncoders();
    odo = base.getOdometry();
    std::cout << "Odometry start values:\nx: " << odo.x
        << " , y: " << odo.y << " , ang: " << odo.ang << "\n";
    
    base.setVelocity(0.0F, 1.0F, 0.0F);
    base.sendSpeed();
    
    sleep(2); // Run motors for 2 seconds

    base.setVelocity(0.0F, 0.0F, 0.0F);
    base.sendSpeed();
    // Stop motors (speed = 0)
    
    base.readEncoders();
    odo = base.getOdometry();
    std::cout << "Odometry end values:\nx: " << odo.x
        << " , y: " << odo.y << " , ang: " << odo.ang << "\n";
    
    std::cout << "Motors ran for 2 seconds, now stopped." << std::endl;
    return 0;
}

