#include "arm.h"

void Arm::DK(float q_1, float q_2, float q_3)
{
    q1 = q_1;
    q2 = q_2;
    q3 = q_3;
    px = l1 * cos(q1) + l2 * cos(q1 + q2 - M_PI) + l3 * cos(q1 + q3 - M_PI);
    py = l1 * sin(q1) + l2 * sin(q1 + q2 - M_PI) + l3 * sin(q1 + q3 - M_PI);
    theta = q3 - M_PI + alpha();
    q2m(q1, q2, q3, m1, m2, m3);
}

int Arm::IK(float p_x, float p_y, float th)
{
    

    float w_x = p_x - l3 * cos(th);
    float w_y = p_y - l3 * sin(th);

    float wl = sqrt(wx*wx + wy*wy);
    if(wl >= (l1+l2 - MARGIN)) return -1;

    px = p_x;
    py = p_y;
    theta = th;
    wx = w_x;
    wy = w_y;

    q2 = acos((l1*l1 + l2*l2 - wx*wx - wy*wy)/(2.0f * l1 * l2));
    q1 = atan2(wy, wx) + asin((l2 * sin(q2))/sqrt(wx*wx + wy*wy));
    
    q3 = M_PI - alpha() + theta;
    q2m(q1, q2, q3, m1, m2, m3);

    //std::cout << q1*180.0f/M_PI << " " << q2*180.0f/M_PI << " " << q3*180.0f/M_PI << "\n"; 

    return 0;

}

void Arm::DK(float q_1, float q_2, float q_3, float &p_x, float &p_y, float &th)
{
    float a = q_1;
    p_x = l1 * cos(q_1) + l2 * cos(q_1 + q_2 - M_PI) + l3 * cos(q_1 + q_3 - M_PI);
    p_y = l1 * sin(q_1) + l2 * sin(q_1 + q_2 - M_PI) + l3 * sin(q_1 + q_3 - M_PI);
    th = q_3 - M_PI + a;
}
int  Arm::IK(float p_x, float p_y, float th, float &q_1, float &q_2, float &q_3)
{
    float w_x = p_x - l3 * cos(th);
    float w_y = p_y - l3 * sin(th);

    float wl = sqrt(wx*wx + wy*wy);
    if(wl >= (l1+l2 - MARGIN)) return -1;

    q_2 = acos((l1*l1 + l2*l2 - w_x*w_x - w_y*w_y)/(2.0f * l1 * l2));
    q_1 = atan2(w_y, w_x) + asin((l2 * sin(q_2))/sqrt(w_x*w_x + w_y*w_y));
    
    float a = q_1;

    q_3 = M_PI - a + theta;
}

int Arm::move(float p_x, float p_y, float th)
{
    int err = 0;
    err = IK(p_x, p_y, th);
    if(err == -1) return -1;

    servos.writePos(11, radToPos(m1));
    servos.writePos(30, radToPos(m2));
    servos.writePos(40, radToPos(m3));
    return 0;
}

int Arm::moveIncr(float dx, float dy, float dth)
{
    int err = 0;
    float p_x = px + dx;
    float p_y = py + dy;
    float th = theta + dth;
    err = IK(p_x, p_y, th);
    if(err == -1) return -1;

    servos.writePos(11, radToPos(m1));
    servos.writePos(30, radToPos(m2));
    servos.writePos(40, radToPos(m3));
    return 0;
}

void Arm::home()
{
    q1 = M_PI/2.0f;
    q2 = M_PI/2.0f;
    q3 = M_PI/2.0f;

    DK(q1, q2, q3);

    q2m(q1, q2, q3, m1, m2, m3);

    servos.writePos(11, radToPos(m1));
    servos.writePos(30, radToPos(m2));
    servos.writePos(40, radToPos(m3));
}