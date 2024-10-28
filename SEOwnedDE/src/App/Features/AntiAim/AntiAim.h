#ifndef ANTI_AIM_H
#define ANTI_AIM_H

#include "../src/Utils/Utils.h"
#include "../src/SDK/SDK.h"
#include "../CFG.h"



class CAntiAim {
public:
    void Main(CUserCmd* pCmd, bool* pSendPacket);
    bool* pSendPacket = reinterpret_cast<bool*>(uintptr_t(_AddressOfReturnAddress()) + 0x128);

private:
   void movementFix(CUserCmd* pCmd, QAngle vOldAngles, float fOldSidemove, float fOldForward);
};

MAKE_SINGLETON_SCOPED(CAntiAim, AntiAim, F)

#endif // ANTI_AIM_H
