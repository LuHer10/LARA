#include "../robot/arm.h"
#include <unistd.h>

int main()
{
    Arm arm(0.4f, 0.4f, 0.1f);
    sleep(2);

    arm.moveIncr(0.1f, 0, 0);
    sleep(1);
    arm.moveIncr(0.1f, 0, 0);
    sleep(1);
    arm.moveIncr(-0.1f, 0, 0);
    sleep(1);
    arm.moveIncr(-0.1f, 0, 0);
    sleep(1);
    arm.moveIncr(-0.1f, 0, 0);
    sleep(1);
    arm.moveIncr(-0.1f, 0, 0);
    sleep(1);
    arm.moveIncr(0.1f, 0, 0);
    sleep(1);
    arm.moveIncr(0.1f, 0, 0);
    sleep(1);

}