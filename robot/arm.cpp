#include "arm.h"

int Arm::DK(float q_1, float q_2, float q_3)
{
    float p_x, p_y, th;

    int err = DK(q_1, q_2,  q_3, p_x, p_y, th);
    //if(err) return err;

    q1 = q_1;
    q2 = q_2;
    q3 = q_3;

    px = p_x;
    py = p_y;
    theta = th;

    q2m(q1, q2, q3, m1, m2, m3);

    return 0;
}

int Arm::IK(float p_x, float p_y, float th)
{
    
    float q_1, q_2, q_3;
    int err = IK(p_x, p_y, th, q_1, q_2, q_3);

    //if(err) return err;

    if(std::isnan(q_1) || std::isnan(q_2) || std::isnan(q_3)) return -1;
    

    px = p_x;
    py = p_y;
    theta = th;
    
    q1 = q_1;
    q2 = q_2;
    q3 = q_3;
    
    q2m(q1, q2, q3, m1, m2, m3);

    //std::cout << q1*180.0f/M_PI << " " << q2*180.0f/M_PI << " " << q3*180.0f/M_PI << "\n"; 

    return 0;

}

int Arm::DK(float q_1, float q_2, float q_3, float &p_x, float &p_y, float &th)
{
    int err = 0;//test_qs(q_1, q_2, q_3);
    //if(err == -1) return -1;
    float alpha = q_1;
    p_x = l1 * cos(q_1) + l2 * cos(q_1 + q_2 - M_PI) + l3 * cos(q_1 + q_3 - M_PI);
    p_y = l1 * sin(q_1) + l2 * sin(q_1 + q_2 - M_PI) + l3 * sin(q_1 + q_3 - M_PI);
    th = q_3 - M_PI + alpha;
    return 0;
}
int Arm::IK(float p_x, float p_y, float th, float &q_1, float &q_2, float &q_3)
{
    float w_x = p_x - l3 * cos(th);
    float w_y = p_y - l3 * sin(th);

    float wl = sqrt(wx*wx + wy*wy);
    //if(wl >= (l1+l2+l3 - MARGIN)) return -1;

    q_2 = acos((l1*l1 + l2*l2 - w_x*w_x - w_y*w_y)/(2.0f * l1 * l2));
    q_1 = atan2(w_y, w_x) + asin((l2 * sin(q_2))/sqrt(w_x*w_x + w_y*w_y));
    
    float alpha = q_1;

    q_3 = M_PI - alpha + theta;

    int err = 0; //test_qs(q_1, q_2, q_3);
    if(std::isnan(q_1) || std::isnan(q_2) || std::isnan(q_3)) return -1;
    return err;

}

int Arm::move(float p_x, float p_y, float th)
{
    int err = 0;
    err = IK(p_x, p_y, th);
    //if(err == -1) return -1;

    writePos(m1, m2, m3);
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

    writePos(m1, m2, m3);
    return 0;
}

void Arm::writePos(float m_1, float m_2, float m_3)
{
    servos.writePos(M1_ID, radToPos(m1));
    servos.writePos(M2_ID, radToPos(m2));
    servos.writePos(M3_ID, radToPos(m3));
}

void Arm::home()
{
    q1 = M_PI/2.0f;
    q2 = M_PI/2.0f;
    q3 = M_PI/2.0f;

    DK(q1, q2, q3);

    q2m(q1, q2, q3, m1, m2, m3);

    writePos(m1, m2, m3);
}

int Arm::test_qs(float q_1, float q_2, float q_3)
{
    if(q_2 >= 135.0f*M_PI/180.0f) return -1;
    if(q_2 <= 45.0f*M_PI/180.0f) return -1;
    if(q_1 >= 170.0f*M_PI/180.0f) return -1;
    if(q_1 <= 10.0f*M_PI/180.0f) return -1;
    if(q_3 >= 135.0f*M_PI/180.0f) return -1;
    if(q_3 <= 45.0f*M_PI/180.0f) return -1;

    return 0;
}


int Arm::move_qs(float q_1, float q_2, float q_3)
{
    //q1 = q_1;
    //q2 = q_1;
    //q3 = q_1;

    q2m(q_1, q_2, q_3, m1, m2, m3);

    writePos(m1, m2, m3);
    return 0;
}

int Arm::moveIncr_ms(float dq1, float dq2, float dq3)
{
    m1 += dq1*q1_2_m1;
    m2 += dq2*q2_2_m2;
    m3 += dq3*q3_2_m3;
    
    //q2m(q1, q2, q3, m1, m2, m3);

    writePos(m1, m2, m3);
    return 0;
}

int Arm::setOffset()
{
    offset1 = m1*m1_2_q1 - 180.0f;
    offset2 = m2*m2_2_q2 - 180.0f;
    offset3 = m3*m3_2_q3 - 180.0f;
    
}
