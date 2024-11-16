#pragma once

#include "../../../SDK/SDK.h"
#include <vector>

class CMisc
{
public:
    void Bunnyhop(CUserCmd* pCmd);
    void AutoStrafer(CUserCmd* pCmd);
    void NoiseMakerSpam();
    void RageRetry();
    void AutoPeek(CUserCmd* pCmd);
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
    // Variables for AutoPeek feature
    std::vector<Vec3> savedPeekPositions;  // List to store multiple saved spots
    bool bReturning = false;  // Flag to check if we're returning to a spot
    int currentSpotIndex = -1;  // Index of the current spot to return to

    // Define bFastAccel as a boolean variable, defaulting to false
    bool bFastAccel = false;
};

// Singleton definition macro (assuming it's defined elsewhere)
MAKE_SINGLETON_SCOPED(CMisc, Misc, F);
