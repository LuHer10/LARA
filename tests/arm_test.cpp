#include "../robot/arm.h"
#include <unistd.h>

int main()
{
    Arm arm(0.4f, 0.4f, L_GRIPPER);
    sleep(2);
    
    arm.moveIncr(0.1f, 0.00f, 0.0f);
    sleep(6);
    arm.moveIncr(0.0f, 0.1f, 0.0f);
    sleep(6);
    arm.moveIncr(-0.1f, 0.00f, 0.0f);
    sleep(6);
    arm.moveIncr(0.0f, -0.1f, 0.0f);
    sleep(6);
    

}