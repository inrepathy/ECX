#pragma once

#include "../../../SDK/SDK.h"

class CMisc
{
public:
    void Bunnyhop(CUserCmd* pCmd);
    void AutoStrafer(CUserCmd* pCmd);
    void NoiseMakerSpam();
    void FastStop(CUserCmd* pCmd);
    void FakeDuck(CUserCmd* pCmd);
    void FastAccel(CUserCmd* pCmd);
    void AutoScoutJump(CUserCmd* pCmd);

    void AutoRocketJump(CUserCmd* cmd);
    void AutoDisguise(CUserCmd* cmd);
    void AutoMedigun(CUserCmd* cmd);
    void MovementLock(CUserCmd* cmd);
    void MvmInstaRespawn();

private:
    // Define bFastAccel as a boolean variable, defaulting to false
    bool bFastAccel = false;
};

// Singleton definition macro (assuming it's defined elsewhere)
MAKE_SINGLETON_SCOPED(CMisc, Misc, F);
