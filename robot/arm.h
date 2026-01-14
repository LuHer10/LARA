#ifndef ARM_H
#define ARM_H
#include <cmath>
#include "servo.h"
#include <iostream>

#define q1_2_m1 1.0f
#define q2_2_m2 2.0f
#define q3_2_m3 2.0f

#define MARGIN 0.01f

class Arm
{
private:

    Servo servos;

    float l1, l2, l3;
    float q1, q2, q3;
    float theta;
    float px, py;
    float wx, wy;
    
    float m1, m2, m3;

    float alpha(){return q1;}

public:

    Arm(float l_1, float l_2, float l_3)
    {
        servos.init();
        servos.enableTorque(10);
        servos.enableTorque(20);
        servos.enableTorque(30);
        servos.enableTorque(40);

        l1 = l_1;
        l2 = l_2;
        l3 = l_3;

        q1 = M_PI/2.0f;
        q2 = M_PI/2.0f;
        q3 = M_PI/2.0f;

        m1 = q1 * q1_2_m1;
        m2 = q2 * q2_2_m2;
        m3 = q3 * q3_2_m3;

        theta = M_PI/2.0f;

        servos.writePos(11, radToPos(m1));
        servos.writePos(30, radToPos(m2));
        servos.writePos(40, radToPos(m3));

        DK(q1, q2, q3);
    }

    ~Arm()
    {
        servos.disableTorque(10);
        servos.disableTorque(20);
        servos.disableTorque(30);
        servos.disableTorque(30);
    }

    void setP(float p_x, float p_y)
    {
        px = p_x;
        py = p_y;
    }
    void setTheta(float th){theta = th;}
    float getPx(){return px;}
    float getPy(){return py;}

    void DK(float q_1, float q_2, float q_3);
    int IK(float p_x, float p_y, float th);

    int move(float p_x, float p_y, float th);
    int moveIncr(float dx, float dy, float dth);

    void home();
    
    void q2m()
    {
        m1 = q1 * q1_2_m1;
        m2 = q2 * q2_2_m2;
        m3 = q3 * q3_2_m3;
    }


};

#endif
