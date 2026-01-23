/*

Motor with ID 10 on the base left  side (M1)
Motor with ID 20 on the base right side (M1)
Motor with ID 30 on the arm  left  side (M3)
Motor with ID 40 on the arm  right side (M2)

*/

#ifndef ARM_H
#define ARM_H
#include <cmath>
#include "servo.h"
#include <iostream>

#define q1_2_m1 1.0f
#define q2_2_m2 2.0f
#define q3_2_m3 2.0f

#define M1_ID 11
#define M2_ID 40
#define M3_ID 30
#define GRIP_ID 50

#define GRIPPER_OPEN  3.0f*M_PI/4.0f
#define GRIPPER_CLOSE M_PI/2.0f

#define MARGIN 0.02f

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

    int DK(float q_1, float q_2, float q_3);
    int IK(float p_x, float p_y, float th);

public:

    Arm(float l_1, float l_2, float l_3)
    {
        servos.init();

        servos.extPosContMode(M1_ID);
        servos.extPosContMode(M2_ID);
        servos.extPosContMode(M3_ID);

        servos.enableTorque(M1_ID);
        servos.enableTorque(M2_ID);
        servos.enableTorque(M3_ID);
        servos.enableTorque(GRIP_ID);

        l1 = l_1;
        l2 = l_2;
        l3 = l_3;

        q1 = M_PI/2.0f;
        q2 = M_PI/2.0f;
        q3 = M_PI/2.0f;

        q2m(q1, q2, q3, m1, m2, m3);

        theta = 0.0f;

        servos.writePos(M1_ID, radToPos(m1));
        servos.writePos(M2_ID, radToPos(m2));
        servos.writePos(M3_ID, radToPos(m3));

        DK(q1, q2, q3);
    }

    ~Arm()
    {
        servos.disableTorque(M1_ID);
        servos.disableTorque(M2_ID);
        servos.disableTorque(M3_ID);
        servos.disableTorque(GRIP_ID);
    }

    void setP(float p_x, float p_y)
    {
        px = p_x;
        py = p_y;
    }
    void setTheta(float th){theta = th;}
    float getPx(){return px;}
    float getPy(){return py;}
    float getTheta(){return theta;}

    void get_qs(float &q_1, float &q_2, float &q_3){q_1 = q1; q_2 = q2; q_3 = q3;}
    void get_pos(float &p_x, float &p_y, float &th){p_x = px; p_y = py; th = theta;}

    

    int DK(float q_1, float q_2, float q_3, float &p_x, float &p_y, float &th);
    int IK(float p_x, float p_y, float th, float &q_1, float &q_2, float &q_3);

    int move(float p_x, float p_y, float th);
    int moveIncr(float dx, float dy, float dth);

    int move_qs(float q_1, float q_2, float q_3);
    int setOffset(float off1, float off2, float off3);

    void home();
    
    void q2m(float q_1, float q_2, float q_3, float &m_1, float &m_2, float &m_3)
    {
        m_1 = q_1 * q1_2_m1;
        m_2 = q_2 * q2_2_m2;
        m_3 = q_3 * q3_2_m3;
    }

    int test_qs(float q_1, float q_2, float q_3);

    void open(){servos.writePos(GRIP_ID, degToPos(135));}
    void close(){servos.writePos(GRIP_ID, degToPos(90));}


};

#endif
