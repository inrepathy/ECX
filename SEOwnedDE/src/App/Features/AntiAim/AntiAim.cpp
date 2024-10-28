#include "AntiAim.h"
#include "../src/Utils/Utils.h"
#include "../src/SDK/SDK.h"

#define M_PI 3.14159265358979323846
#define M_RADPI 57.295779513082

#include "../CFG.h"

void CAntiAim::movementFix(CUserCmd* pCmd, QAngle vOldAngles, float fOldSidemove, float fOldForward)
{
    QAngle curAngs = pCmd->viewangles;
    float deltaView = pCmd->viewangles.y - vOldAngles.y;
    float f1;
    float f2;

    if (vOldAngles.y < 0.f)
        f1 = 360.0f + vOldAngles.y;
    else
        f1 = vOldAngles.y;

    if (pCmd->viewangles.y < 0.0f)
        f2 = 360.0f + pCmd->viewangles.y;
    else
        f2 = pCmd->viewangles.y;

    if (f2 < f1)
        deltaView = abs(f2 - f1);
    else
        deltaView = 360.0f - abs(f1 - f2);
    deltaView = 360.0f - deltaView;

    pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
    pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

void CAntiAim::Main(CUserCmd* pCmd, bool* pSendPacket)
{
    Vec3 vecTargetAngle = pCmd->viewangles;

    bool bIsShooting = (pCmd->buttons & IN_ATTACK);

    if (bIsShooting)
    {
        G::bCanPrimaryAttack = true;
        return;
    }
    else
    {
        G::bCanPrimaryAttack = false; 
    }

    QAngle vOldAngles = pCmd->viewangles;
    float fOldSideMove = pCmd->sidemove;
    float fOldForward = pCmd->forwardmove;

    if (CFG::AntiAim)
    {
        bool bSilentAngles = CFG::AntiAim;
        bool bFakeAngle = CFG::AntiAim_FakeAngle;
        bool bJitterPitch = CFG::AntiAim_JitterPitch; 

        if (bFakeAngle) {
            *pSendPacket = false;

            pCmd->viewangles.y += CFG::AntiAim_Fake_Yaw;
            pCmd->viewangles.x = CFG::AntiAim_Fake_Pitch;

            G::bSilentAngles = true;

            pCmd->viewangles.x = Math::NormalizeAngle(pCmd->viewangles.x);
            pCmd->viewangles.y = Math::NormalizeAngle(pCmd->viewangles.y);

            if (bJitterPitch)
            {
                static bool bJitterSwitch = false;
                float jitterAmount = bJitterSwitch ? 89.f : -89.f;
                bJitterSwitch = !bJitterSwitch;

                pCmd->viewangles.x = jitterAmount;

                if (CFG::AntiAim_FakeAngle) {
                    *pSendPacket = false;
                }
            }

            Math::ClampAngles(pCmd->viewangles);
        }
        else
        {
            if (CFG::AntiAim_Left)
            {
                pCmd->viewangles.x -= CFG::AntiAim_Pitch;
            }
            else
            {
                pCmd->viewangles.x += CFG::AntiAim_Pitch;
            }

            pCmd->viewangles.y += CFG::AntiAim_Yaw;

            if (bJitterPitch)
            {
                static bool bJitterSwitch = false;
                float jitterAmount = bJitterSwitch ? 89.f : -89.f;
                bJitterSwitch = !bJitterSwitch;

                pCmd->viewangles.x = jitterAmount;

                if (CFG::AntiAim_FakeAngle) {
                    *pSendPacket = false;
                }
            }

            if (bSilentAngles)
            {
                Vec3 vSilentAngle = {};
                Math::VectorAngles(pCmd->viewangles, vSilentAngle);

                pCmd->viewangles.x = CFG::AntiAim_Pitch;
                pCmd->viewangles.y = vSilentAngle.y;

                G::bSilentAngles = true;
                *pSendPacket = true;
            }
            else
            {
                G::bSilentAngles = false;
                *pSendPacket = false;
            }
        }

        CAntiAim::movementFix(pCmd, vOldAngles, fOldSideMove, fOldForward);

        Math::ClampAngles(pCmd->viewangles);
    }
}
