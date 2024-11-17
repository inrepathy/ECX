#include "../../SDK/SDK.h"

#include "../Features/CFG.h"
#include "../Features/NetworkFix/NetworkFix.h"
#include "../Features/SeedPred/SeedPred.h"

MAKE_SIGNATURE(CL_Move, "engine.dll", "40 55 53 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 83 3D", 0x0);

MAKE_HOOK(CL_Move, Signatures::CL_Move.Get(), void, __fastcall,
    float accumulated_extra_samples, bool bFinalTick)
{
    auto callOriginal = [&](bool bFinal)
        {
            if (CFG::Misc_Ping_Reducer)
            {
                F::NetworkFix->FixInputDelay(bFinal);
            }

            F::SeedPred->AskForPlayerPerf();

            if (Shifting::nAvailableTicks < MAX_COMMANDS)
            {
                if (auto pWeapon = H::Entities->GetWeapon(); pWeapon)
                {
                    if (!Shifting::bRecharging && !Shifting::bShifting && !Shifting::bShiftingWarp)
                    {
                        if (H::Input->IsDown(CFG::Exploits_Shifting_Recharge_Key))
                        {
                            Shifting::bRecharging = !I::MatSystemSurface->IsCursorVisible() &&
                                !I::EngineVGui->IsGameUIVisible();
                        }
                    }
                }

                if (Shifting::bRecharging)
                {
                    Shifting::nAvailableTicks++;
                    return;
                }
            }
            else
            {
                Shifting::bRecharging = false;
            }

            CALL_ORIGINAL(accumulated_extra_samples, bFinal);
        };

    auto handleShift = [&](int nTicks)
        {
            Shifting::bShifting = true;

            for (int n = 0; n < nTicks; n++)
            {
                callOriginal(n == nTicks - 1);
                Shifting::nAvailableTicks--;
            }

            Shifting::bShifting = false;
        };

    if (Shifting::bRapidFireWantShift)
    {
        Shifting::bRapidFireWantShift = false;
        handleShift(CFG::Exploits_RapidFire_Ticks);
        return;
    }

    if (auto pLocal = H::Entities->GetLocal(); pLocal && !pLocal->deadflag())
    {
        if (!Shifting::bRecharging && !Shifting::bShifting && !Shifting::bShiftingWarp)
        {
            if (!I::MatSystemSurface->IsCursorVisible() &&
                !I::EngineVGui->IsGameUIVisible() &&
                H::Input->IsDown(CFG::Exploits_Warp_Key) &&
                Shifting::nAvailableTicks)
            {
                Shifting::bShiftingWarp = true;

                if (CFG::Exploits_Warp_Mode == 0)
                {
                    handleShift(2); // warp mode 0 uses 2 ticks
                }
                else if (CFG::Exploits_Warp_Mode == 1)
                {
                    handleShift(Shifting::nAvailableTicks); // warp mode 1 uses all available ticks
                }

                Shifting::bShiftingWarp = false;
                return;
            }
        }
    }

    callOriginal(bFinalTick);
}
