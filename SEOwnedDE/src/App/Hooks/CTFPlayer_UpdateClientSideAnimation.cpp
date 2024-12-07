#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_HOOK(CTFPlayer_UpdateClientSideAnimation, Signatures::CTFPlayer_UpdateClientSideAnimation.Get(), void, __fastcall,
	C_TFPlayer* ecx)
{
	if (CFG::Misc_Accuracy_Improvements)
	{
		if (const auto pLocal = H::Entities->GetLocal())
		{
			if (ecx == pLocal)
			{
				if (!pLocal->InCond(TF_COND_HALLOWEEN_KART))
				{
					if (const auto pWeapon = H::Entities->GetWeapon())
					{
						pWeapon->UpdateAllViewmodelAddons(); //credits: KGB
					}

					return;
				}
				CALL_ORIGINAL(ecx);
			}
		}

		if (!G::bUpdatingAnims)
		{
			return;
		}
	}

	CALL_ORIGINAL(ecx);
}
