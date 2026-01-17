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

    int8_t grip;

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

    while (1) {

        pre_x_left = x_left;
        pre_y_left = y_left;
        pre_x_right = x_right;
        pre_y_right = y_right;

        net.receiveJoysticks(x_left, y_left, x_right, y_right, grip);

        base.readEncoders();

        odo = base.getOdometry();

        net.sendOdometry(odo.x, odo.y, odo.ang);

        arm.get_qs(q1, q2, q3);
        arm.DK(q1, q2, q3, arm_x, arm_y, arm_th);

        changed = (pre_x_left != x_left)
                || (pre_y_left != y_left)
                || (pre_x_right != x_right)
                || (pre_y_right != y_right);

        if(changed)
        {
            if(arm_x > 0.7f || arm_x < 0.3f)
            {
                base.setVelocity(x_left*0.5f, y_left*0.5f, -x_right);
                base.sendSpeed();
                arm.moveIncr(0.0f, y_right*0.1f, 0.0f);
            }
            else
            {
                arm.moveIncr(y_left*0.1f, y_right*0.1f, 0.0f);
            }

            
        }

        //sleep(10);  // 10 Hz loop
    }

    return 0;
}
