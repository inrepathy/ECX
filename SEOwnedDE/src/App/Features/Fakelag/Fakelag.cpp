#include "../src/SDK/SDK.h"
#include "Fakelag.h"
#include "../src/Utils/Utils.h"
#include "../src/App/Features/CFG.h"

void CFakelag::Run(CUserCmd* pCmd, bool* pSendPacket) {
    if (CFG::Fakelag) {
        const int maxChokeTicks = std::min(CFG::Fakelag_Ticks, 22);
        static int chokeTicks = 0;

        bool bIsShooting = (pCmd->buttons & IN_ATTACK);

        if (bIsShooting) {
            if (CFG::Fakelag_DisableWhileFiring) {
                *pSendPacket = true;
                chokeTicks = 0;
                G::bCanPrimaryAttack = true;
                return;
            }

            if (CFG::Fakelag_DisableIfDT) {
                if (H::Input->IsDown(CFG::Exploits_RapidFire_Key)) {
                    *pSendPacket = true;
                    chokeTicks = 0;
                    G::bCanPrimaryAttack = true;
                    return;
                }
            }
        }
        else {
            G::bCanPrimaryAttack = false; 
        }

        if (chokeTicks < maxChokeTicks) {
            *pSendPacket = false; 
            chokeTicks++;
        }
        else {
            *pSendPacket = true; 
            chokeTicks = 0; 
        }
    }
}
