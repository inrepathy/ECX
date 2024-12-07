#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CTFWearable_ShouldDraw, "client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC ? 8B 91", 0x0);
MAKE_SIGNATURE(CTFWearable_InternalDrawModel, "client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 44 8B 81", 0x0);

MAKE_HOOK(CTFWearable_ShouldDraw, Signatures::CTFWearable_ShouldDraw.Get(), bool, __fastcall,
	void* ecx)
{
	if (CFG::Visuals_Disable_Wearables)
		return false;

	return CALL_ORIGINAL(ecx);
}

MAKE_HOOK(CTFWearable_InternalDrawModel, Signatures::CTFWearable_InternalDrawModel.Get(), int, __fastcall,
	void* ecx, int flags)
{
	if (CFG::Visuals_Disable_Wearables)
		return 0;

	return CALL_ORIGINAL(ecx, flags);
}