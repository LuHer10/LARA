#include "../robot/servo.h"
#include <unistd.h>

int main()
{
    Servo servos;
    servos.init();

    servos.enableTorque(11);
    servos.enableTorque(30);
    servos.enableTorque(40);

    servos.writePos(11, degToPos(90));
    servos.writePos(30, degToPos(180));
    servos.writePos(40, degToPos(180));
    sleep(3);
    servos.disableTorque(10);
    servos.disableTorque(20);
    servos.disableTorque(30);
    servos.disableTorque(40);
}