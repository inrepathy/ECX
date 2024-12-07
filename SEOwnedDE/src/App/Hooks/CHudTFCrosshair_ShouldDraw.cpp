#include "../../SDK/SDK.h"

MAKE_SIGNATURE(CHudTFCrosshair_ShouldDraw, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 8B 15 ? ? ? ? 48 8B D9 48 85 D2", 0x0);

MAKE_HOOK(CHudTFCrosshair_ShouldDraw, Signatures::CHudTFCrosshair_ShouldDraw.Get(), bool, __fastcall,
	void* ecx)
{
	if (G::bStartedFakeTaunt)
	{
		return false;
	}

	return CALL_ORIGINAL(ecx);
}
