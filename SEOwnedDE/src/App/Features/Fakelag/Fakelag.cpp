#include "../src/SDK/SDK.h"
#include "Fakelag.h"
#include "../src/Utils/Utils.h"
#include "../src/App/Features/CFG.h"
#include <random>

void CFakelag::Run(CUserCmd* pCmd, bool* pSendPacket) {
    if (CFG::Fakelag) {

        if (CFG::Fakelag_Style == 0) {
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

                if (CFG::Fakelag_DisableifFD) {
                    if (H::Input->IsDown(CFG::FakeDuck_Key)) {
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

        if (CFG::Fakelag_Style == 1) {
            static int chokeTicks = 0;
            int maxChokeTicks = std::min(CFG::Fakelag_Ticks, 22);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(1, maxChokeTicks);

            bool bIsMoving = (pCmd->forwardmove != 0 || pCmd->sidemove != 0);

            if (bIsMoving) {
                maxChokeTicks = dist(gen);
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
}


