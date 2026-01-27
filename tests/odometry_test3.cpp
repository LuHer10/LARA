#include "../robot/base.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>

#define SERIAL_PORT "/dev/ttyAMA0"
#define BAUD_RATE B38400

#define INTERVAL_US 200000  // 200 ms in microseconds
#define DIR_CHANGE_US 2000000

long time_diff_us(struct timeval a, struct timeval b) {
    return (a.tv_sec - b.tv_sec) * 1000000L +
           (a.tv_usec - b.tv_usec);
}

int main() {
    
    //RoboClaw rcl(SERIAL_PORT, BAUD_RATE);
    Base base;

    Odometry odo;
    
    base.readEncoders();
    odo = base.getOdometry();
    std::cout << "Odometry start values:\nx: " << odo.x
        << " , y: " << odo.y << " , ang: " << odo.ang << "\n";

    float pre_x = odo.x; 
    float pre_y = odo.y;


    FILE *file = fopen("odometry.csv", "a");
    if (!file) {
        perror("fopen");
        return 1;
    }

    fprintf(file, "Test3\nTime; X; Y; Angle; VX; VY\n");
    fflush(file);

    struct timeval last, now, moveLast;
    gettimeofday(&last, NULL);
    gettimeofday(&moveLast, NULL);

    int count = 0;

    base.setVelocity(0.0f, 10.0f, 0.0f);
    base.sendSpeed();

    while (1) {
        gettimeofday(&now, NULL);
        odo = base.getOdometry();

        if (time_diff_us(now, last) >= INTERVAL_US) {
            odo = base.getOdometry();
            fprintf(file, "%d; %f; %f; %f; %f; %f\n", now.tv_sec * 1000000L + now.tv_usec, 
                odo.x, odo.y, odo.ang, odo.x-pre_x, odo.y-pre_y);
            fflush(file);

            last = now;  // reset reference time

            pre_x = odo.x;
            pre_y = odo.y;

        }

        if(odo.y >= 5.0f)
        {
            base.setVelocity(0.0f, 0.0f, 0.0f);
            base.sendSpeed();
            break;
        }

        

    }

    fclose(file);
    
    base.readEncoders();
    odo = base.getOdometry();
    std::cout << "Odometry end values:\nx: " << odo.x
        << " , y: " << odo.y << " , ang: " << odo.ang << "\n";
    
    return 0;
}

