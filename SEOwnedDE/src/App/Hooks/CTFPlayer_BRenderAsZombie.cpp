#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CTFPlayer_BRenderAsZombie, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 B9 ? ? ? ? E8 ? ? ? ? 84 C0", 0x0);
MAKE_SIGNATURE(CTFRagdoll_CreateTFRagdoll_CTFPlayer_BRenderAsZombie_Call, "client.dll", "E8 ? ? ? ? 84 C0 74 ? C6 87", 0x5);

MAKE_HOOK(CTFPlayer_BRenderAsZombie, Signatures::CTFPlayer_BRenderAsZombie.Get(), bool, __fastcall,
	C_TFPlayer* ecx, bool bWeaponsCheck)
{
	if (CFG::Visuals_Ragdolls_Active && CFG::Visuals_Ragdolls_No_Gib)
	{
		if (reinterpret_cast<std::uintptr_t>(_ReturnAddress()) == Signatures::CTFRagdoll_CreateTFRagdoll_CTFPlayer_BRenderAsZombie_Call.Get())
			return true;
	}

	return CALL_ORIGINAL(ecx, bWeaponsCheck);
}