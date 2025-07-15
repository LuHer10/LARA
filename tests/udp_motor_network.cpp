#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "../robot/base.h"
#include "../robot/network.h"


int main() {

    Network net;
    
    float x_left = 0.0F;
    float y_left = 0.0F;
    float x_right = 0.0F;
    float y_right = 0.0F;

    float pre_x_left, pre_y_left;
    float pre_x_right, pre_y_right;

    bool changed = false;

    Odometry odo;

    Base base;
    base.readEncoders();
    odo = base.getOdometry();

    while (1) {

        pre_x_left = x_left;
        pre_y_left = y_left;
        pre_x_right = x_right;
        pre_y_right = y_right;

        net.receiveJoysticks(x_left, y_left, x_right, y_right);

        base.readEncoders();

        odo = base.getOdometry();

        net.sendOdometry(odo.x, odo.y, odo.ang);

        changed = (pre_x_left != x_left)
                || (pre_y_left != y_left)
                || (pre_x_right != x_right)
                || (pre_y_right != y_right);

        if(changed)
        {
            base.setVelocity(x_left*0.5f, y_left*0.5f, -x_right);
            base.sendSpeed();
        }

        //sleep(10);  // 10 Hz loop
    }

    return 0;
}
