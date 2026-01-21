#include "../robot/servo.h"
#include <unistd.h>

int main()
{
    Servo servos;
    servos.init();

    //servos.extPosContMode(10);
    //servos.extPosContMode(20);
    //servos.extPosContMode(30);
    //servos.extPosContMode(40);

    servos.enableTorque(11);
    servos.enableTorque(30);
    servos.enableTorque(40);

    servos.writePos(11, degToPos(0));
    servos.writePos(30, degToPos(0));
    servos.writePos(40, degToPos(0));
    sleep(3);
    servos.disableTorque(11);
    servos.disableTorque(30);
    servos.disableTorque(40);
}