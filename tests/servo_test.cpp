#include "../robot/servo.h"
#include <unistd.h>

int main()
{
    Servo servos;
    servos.init();

    servos.enableTorque(11);
    servos.enableTorque(30);
    servos.enableTorque(40);

    servos.writePos(11, 2);
    servos.writePos(30, 2);
    servos.writePos(40, 2);
    sleep(1);
    servos.writePos(11, degToPos(90));
    servos.writePos(30, degToPos(90));
    servos.writePos(40, degToPos(90));
    sleep(1);
    servos.writePos(11, degToPos(180));
    servos.writePos(30, degToPos(180));
    servos.writePos(40, degToPos(180));
    sleep(1);
    servos.writePos(11, degToPos(90));
    servos.writePos(30, degToPos(90));
    servos.writePos(40, degToPos(90));
    sleep(1);
    servos.writePos(11, 2);
    servos.writePos(30, 2);
    servos.writePos(40, 2);
    sleep(1);
    servos.disableTorque(10);
    servos.disableTorque(20);
    servos.disableTorque(30);
    servos.disableTorque(40);
}