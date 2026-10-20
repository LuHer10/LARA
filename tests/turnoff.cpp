#include "../robot/servo.h"
#include <unistd.h>
#include <stdio.h>

int main()
{
    Servo servos;
    servos.init();


    servos.disableTorque(11);
    servos.disableTorque(30);
    servos.disableTorque(40);
    servos.disableTorque(50);

}