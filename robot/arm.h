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

#define m1_2_q1 1.0f/q1_2_m1
#define m2_2_q2 1.0f/q2_2_m2
#define m3_2_q3 1.0f/q3_2_m3

#define M1_ID 11
#define M2_ID 40
#define M3_ID 30
#define GRIP_ID 50

#define GRIPPER_OPEN  3.0f*M_PI/4.0f
#define GRIPPER_CLOSE M_PI/2.0f

#define MARGIN 0.02f

#define L_GRIPPER 0.215f 

#define MAX_VEL 10
#define MAX_ACC 5

class Arm
{
private:

    Servo servos;

    float l1, l2, l3;
    float q1, q2, q3;
    float theta;
    float px, py;
    float wx, wy;

    float offset1 = 0;
    float offset2 = 0;
    float offset3 = 0;
    
    float m1, m2, m3;

    int DK(float q_1, float q_2, float q_3);
    int IK(float p_x, float p_y, float th);

    void writePos(float m_1, float m_2, float m_3);

public:

    Arm(float l_1, float l_2, float l_3)
    {
        servos.init();

        servos.extPosContMode(M1_ID);
        servos.extPosContMode(M2_ID);
        servos.extPosContMode(M3_ID);

        servos.setMaxVel(M1_ID, MAX_VEL);
        servos.setMaxVel(M2_ID, MAX_VEL);
        servos.setMaxVel(M3_ID, MAX_VEL);

        servos.setMaxAcc(M1_ID, MAX_ACC);
        servos.setMaxAcc(M2_ID, MAX_ACC);
        servos.setMaxAcc(M3_ID, MAX_ACC);


        servos.enableTorque(M1_ID);
        servos.enableTorque(M2_ID);
        servos.enableTorque(M3_ID);
        servos.enableTorque(GRIP_ID);

        l1 = l_1;
        l2 = l_2;
        l3 = l_3;

        /**/
        q1 = M_PI/2.0f;
        q2 = M_PI/2.0f;
        q3 = M_PI/2.0f;

        m1 = posToRad(servos.readPos(10));
        //m1 = posToRad(servos.readPos(20));
        m2 = posToRad(servos.readPos(M2_ID));
        m3 = posToRad(servos.readPos(M3_ID));

        printf("%f, %f, %f  \n", m1, m2, m3);

        m2q(m1, m2, m3, q1, q2, q3);


        //q2m(q1, q2, q3, m1, m2, m3);

        theta = 0.0f;

        //writePos(m1, m2, m3);

        DK(q1, q2, q3);
    }

    ~Arm()
    {
        //servos.disableTorque(M1_ID);
        //servos.disableTorque(M2_ID);
        //servos.disableTorque(M3_ID);
        //servos.disableTorque(GRIP_ID);
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
    int moveIncr_ms(float dq1, float dq2, float dq3);
    
    void move_q1(float q_1){
        servos.writePos(M1_ID, radToPos(q_1*q1_2_m1));
    }

    int setOffset();

    void home();
    
    void q2m(float q_1, float q_2, float q_3, float &m_1, float &m_2, float &m_3)
    {
        m_1 = (q_1 + offset1) * q1_2_m1;
        m_2 = (q_2 + offset2) * q2_2_m2;
        m_3 = (q_3 + offset3) * q3_2_m3;
    }

    void m2q(float m_1, float m_2, float m_3, float &q_1, float &q_2, float &q_3)
    {
        q_1 = m_1 * m1_2_q1 - offset1;
        q_2 = m_2 * m2_2_q2 - offset2;
        q_3 = m_3 * m3_2_q3 - offset3;

    }

    int test_qs(float q_1, float q_2, float q_3);

    void open(){servos.writePos(GRIP_ID, degToPos(135));}
    void close(){servos.writePos(GRIP_ID, degToPos(90));}

    


};

#endif
