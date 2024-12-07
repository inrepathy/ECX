#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CTFRagdoll_CreateTFRagdoll, "client.dll", "48 89 4C 24 ? 55 53 56 57 48 8D AC 24 ? ? ? ? B8 ? ? ? ? E8 ? ? ? ? 48 2B E0 8B 91", 0x0);

MAKE_HOOK(CTFRagdoll_CreateTFRagdoll, Signatures::CTFRagdoll_CreateTFRagdoll.Get(), void, __fastcall,
	C_TFRagdoll* ecx)
{
	if (CFG::Visuals_Disable_Ragdolls)
		return;

	if (CFG::Visuals_Ragdolls_Active)
	{
		if (const auto pRagdoll = ecx)
		{
			int nEffect = CFG::Visuals_Ragdolls_Effect;

			if (nEffect == 7)
			{
				nEffect = SDKUtils::RandomInt(0, 6);
			}

			pRagdoll->m_bBurning() = nEffect == 1;
			pRagdoll->m_bElectrocuted() = nEffect == 2;
			pRagdoll->m_bBecomeAsh() = nEffect == 3;
			pRagdoll->m_bGoldRagdoll() = nEffect == 4;
			pRagdoll->m_bIceRagdoll() = nEffect == 5;
			pRagdoll->m_bDissolving() = nEffect == 6;

			pRagdoll->m_vecForce() *= CFG::Visuals_Ragdolls_Force_Mult;

			if (CFG::Visuals_Ragdolls_No_Death_Anim)
			{
				pRagdoll->m_iDamageCustom() = 0;
			}
		}
	}

	CALL_ORIGINAL(ecx);
}
