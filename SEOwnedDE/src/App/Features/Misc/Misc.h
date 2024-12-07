#pragma once

#include "../../../SDK/SDK.h"
#include <vector>

class CMisc
{
public:
    void Bunnyhop(CUserCmd* pCmd);
    void AutoStrafer(CUserCmd* pCmd);
    void NoiseMakerSpam();
    void AutoEdgebug(CUserCmd* pCmd);
    void RageRetry();
    void CircleStrafer(CUserCmd* pCmd);
    void DuckInAir(CUserCmd* pCmd);
    void AutoPeek(CUserCmd* pCmd);
    void FastStop(CUserCmd* pCmd);
    void FakeDuck(CUserCmd* pCmd);
    void FastAccel(CUserCmd* pCmd);
    void AntiAfk(CUserCmd* pCmd);

    void SequenceFreeze(CUserCmd* pCmd, bool *pSendpacket);


    void LegJitter(CUserCmd* pCmd);


    void AutoRocketJump(CUserCmd* cmd);
    void AutoDisguise(CUserCmd* cmd);
    void AutoMedigun(CUserCmd* cmd);
    void MovementLock(CUserCmd* cmd);
    void MvmInstaRespawn();

private:
    std::vector<Vec3> savedPeekPositions;  
    bool bReturning = false; 
    int currentSpotIndex = -1;  
    bool bFastAccel = false;
};

MAKE_SINGLETON_SCOPED(CMisc, Misc, F);
