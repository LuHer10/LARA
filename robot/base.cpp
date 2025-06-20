#include "base.h"

inline void Base::FK(const float vm[4], float &vx, float &vy, float &vrad)
{
    vx   = WHEEL_RADIUS * (-vm[0] + vm[1] + vm[2] - vm[3])/4.0F;
    vy   = WHEEL_RADIUS * ( vm[0] + vm[1] + vm[2] + vm[3])/4.0F;
    vrad = WHEEL_RADIUS * (-vm[0] + vm[1] - vm[2] + vm[3])/(4.0F*LXY);
}

inline void Base::IK(const float &vx, const float &vy, const float &vrad, float vm[4])
{
    vm[0] =  vx + vy - vrad*LXY;
    vm[1] = -vx + vy + vrad*LXY;
    vm[2] = -vx + vy - vrad*LXY;
    vm[3] =  vx + vy + vrad*LXY;
}

void Base::setVelocity(float vx, float vy, float vrad)
{
    float vm[4];
    IK(vx, vy, vrad, vm);
    for(int i = 0; i < 4; i++)
    {
        vm[i] *= RAD_2_QPPR;
        targetVelocity[i] = (int32_t)vm[i];
    }
}

void Base::sendSpeed()
{
    roboclaw.M1M2Speed(FRONT_ADDRESS, targetVelocity[M_FRONT_LEFT], targetVelocity[M_FRONT_RIGHT]);
    roboclaw.M1M2Speed(BACK_ADDRESS, targetVelocity[M_BACK_LEFT], targetVelocity[M_BACK_RIGHT]);
}

void Base::readEncoders()
{
    uint32_t encs[4];
    float angs[4] = {0.0F};
    bool front, back;
    front = roboclaw.readEncoders(FRONT_ADDRESS, encs[M_FRONT_LEFT], encs[M_FRONT_RIGHT]);
    back  = roboclaw.readEncoders( BACK_ADDRESS, encs[M_BACK_LEFT],  encs[M_BACK_RIGHT]);

    if(front && back)
    {
        for(int i = 0; i < 4; i++)
        {
            angs[i] = encs[i] - encoderCounts[i];
            encoderCounts[i] = encs[i];
            angs[i] *= QPPR_2_RAD;
        }

        float d_xr, d_yr, d_ang;
        FK(angs, d_xr, d_yr, d_ang);

        x += d_xr * cos(ang + d_ang/2) - d_yr * sin(ang + d_ang/2);
        y += d_xr * sin(ang + d_ang/2) + d_yr * cos(ang + d_ang/2);
        ang += d_ang;

    }
}

void Base::resetOdometry()
{
    for(int i = 0; i < 4; i++)
        encoderCounts[i] = 0;
    x = y = ang = 0.0F;
    roboclaw.resetEncoders(FRONT_ADDRESS);
    roboclaw.resetEncoders(BACK_ADDRESS);

}