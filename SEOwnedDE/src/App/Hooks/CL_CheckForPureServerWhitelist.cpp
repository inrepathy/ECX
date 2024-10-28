#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CL_CheckForPureServerWhitelist, "engine.dll", "40 56 48 83 EC ? 83 3D ? ? ? ? ? 48 8B F1 0F 8E", 0x0);

MAKE_HOOK(CL_CheckForPureServerWhitelist, Signatures::CL_CheckForPureServerWhitelist.Get(), void, __cdecl,
	void** pFilesToReload)
{
	if (CFG::Misc_Pure_Bypass)
		return;

	CALL_ORIGINAL(pFilesToReload);
}
