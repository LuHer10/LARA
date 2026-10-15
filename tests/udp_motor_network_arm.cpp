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

    int8_t d1, d2, d3;

    int8_t mode;
    int8_t grip = 0;

    int manual = 0;
    int pre_manual = manual;

    float q2off = 0;
    float q3off = 0;

    float pre_x_left, pre_y_left;
    float pre_x_right, pre_y_right;

    bool changed = false;

    Odometry odo;

    Base base;

    Arm arm(0.4f, 0.4f, L_GRIPPER);
    arm.get_qs(q1, q2, q3);
    arm.get_pos(arm_x, arm_y, arm_th);
    
    base.readEncoders();
    odo = base.getOdometry();

    arm.open();

    while (1) {

        pre_x_left = x_left;
        pre_y_left = y_left;
        pre_x_right = x_right;
        pre_y_right = y_right;

        

        net.receiveJoysticks(x_left, y_left, x_right, y_right,
                             d1, d2, d3,
                             mode);

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

        if(changed)
        {
            //if(arm_x > 0.7f || arm_x < 0.3f)
            //{
                base.setVelocity(x_left*5.0f, y_left*5.0f, 0/*-x_right*/);
                base.sendSpeed();
                //arm.moveIncr(0.0f, y_right*0.1f, 0.0f);
            //}
            //else
            //{
                //arm.moveIncr(y_left*0.1f, y_right*0.1f, 0.0f);
            //}

            
        }

        
        if(!manual){
        if(x_right || y_right)
            {
            
                arm.moveIncr(x_right*0.005f, y_right*0.005f, 0.0f);
            }
        }
        else {
            arm.moveIncr_ms(0.01f*d1, 0.01f*d2, 0.01f*d3);
        }

        if(pre_manual != manual && !manual)
        {
            arm.setOffset();
            arm.home();
            //arm.move_qs(PI/2.0f, PI, PI);
        }

        grip = mode & 0x01;

        if(mode & 0x01)
        {
            arm.close();
        }
        else
        {
            arm.open();
        }

        /*
        if(pre_manual != manual && !manual)
        {
            arm.setOffset();
            arm.move_qs(PI/2.0f, PI, PI);
        }
        */

        printf("%f, %f, %f      ", q1*180.0f/PI, q2*180.0f/PI, q3*180.0f/PI);
        printf("%f, %f, %f, %d     ", arm_x, arm_y, arm_th, manual);
        printf("%d, %d, %d  \n", d1, d2, d3);

        //printf("%f, %f, %f, %f, %d", x_left, y_left, x_right, y_right, grip);

        usleep(1000);  // 10 Hz loop
    
    }
    return 0;
}
