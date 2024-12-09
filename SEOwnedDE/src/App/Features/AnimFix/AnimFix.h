#pragma once

#include "../../SEOwnedDE/src/SDK/SDK.h"
#include "../../Features/CFG.h"

class CAnimFix
{
public:
    int TicksElapsed = 0;
    int CTFPlayerAnimState = G::bUpdatingAnims;
    matrix3x4_t BoneMatrix[128];
    matrix3x4_t pMatrixOut;
    void Update(CUserCmd* pCmd);
};

MAKE_SINGLETON_SCOPED(CAnimFix, AnimFix, F)
