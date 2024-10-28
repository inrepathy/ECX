#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CBaseEntity_ResetLatched, "client.dll", "40 56 48 83 EC ? 48 8B 01 48 8B F1 FF 90 ? ? ? ? 84 C0 75", 0x0);

MAKE_HOOK(CBaseEntity_ResetLatched, Signatures::CBaseEntity_ResetLatched.Get(), void, __fastcall,
	void* ecx)
{
	if (CFG::Misc_Pred_Error_Jitter_Fix)
	{
		return;
	}

	CALL_ORIGINAL(ecx);
}
