#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "../robot/base.h"
#include "../robot/network.h"
#include "../robot/arm.h"


int main() {

    Network net;
    
    float x_left = 0.0F;
    float y_left = 0.0F;
    float x_right = 0.0F;
    float y_right = 0.0F;

    float q1, q2, q3;
    float arm_x, arm_y, arm_th;
    float pre_arm_x,  pre_arm_y;

    int8_t mode;

    int manual = 0;
    int pre_manual = manual;

    float q2off = 0;
    float q3off = 0;

    float pre_x_left, pre_y_left;
    float pre_x_right, pre_y_right;

    bool changed = false;

    Odometry odo;

    Base base;

    Arm arm(0.4f, 0.4f, 0.1f);
    arm.get_qs(q1, q2, q3);
    arm.get_pos(arm_x, arm_y, arm_th);
    
    base.readEncoders();
    odo = base.getOdometry();

    int error;

    while (1) {

        pre_x_left = x_left;
        pre_y_left = y_left;
        pre_x_right = x_right;
        pre_y_right = y_right;

        pre_arm_x = arm_x;
        pre_arm_y = arm_y;

        net.receiveJoysticks(x_left, y_left, x_right, y_right, mode);

        pre_manual = manual;

        manual = mode >> 2;

        base.readEncoders();

        odo = base.getOdometry();

        net.sendOdometry(odo.x, odo.y, odo.ang);

        arm.get_qs(q1, q2, q3);
        arm.DK(q1, q2, q3, arm_x, arm_y, arm_th);
        arm.get_pos(arm_x, arm_y, arm_th);

        changed = (pre_x_left != x_left)
                || (pre_y_left != y_left)
                || (pre_x_right != x_right)
                || (pre_y_right != y_right);


        if(y_left || y_right)
        {
            if(manual == 0)
                if(arm >= 0.3 || arm_x > pre_arm_x)
                error = arm.moveIncr(y_left*0.01f, -y_right*0.01f, 0.0f);
            if(manual == 1)
            {
                arm.move_q1(PI/2.0f);
                arm.moveIncr_ms(0.0f, y_left, y_right);
            }
            
        }

        if(pre_manual != manual && !manual)
        {
            arm.setOffset();
            arm.move_qs(PI/2.0f, PI, PI);
        }

        if(changed)
        {
            if(error == -1 || arm < 0.3)
            {
                base.setVelocity(x_left*0.5f, y_left*0.5f, -x_right);
                base.sendSpeed();
            }
        }

        printf("%f, %f, %f      ", q1*180.0f/PI, q2*180.0f/PI, q3*180.0f/PI);
        printf("%f, %f, %f  \n", arm_x, arm_y, arm_th);

        //printf("%f, %f, %f, %f, %d", x_left, y_left, x_right, y_right, grip);

        usleep(100);  // 10 Hz loop
    
    }
    return 0;
}
