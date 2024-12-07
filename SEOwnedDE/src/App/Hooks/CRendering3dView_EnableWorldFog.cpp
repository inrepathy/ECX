#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CRendering3dView_EnableWorldFog, "client.dll", "40 53 48 83 EC ? 48 8B 0D ? ? ? ? 48 89 7C 24", 0x0);

MAKE_HOOK(CRendering3dView_EnableWorldFog, Signatures::CRendering3dView_EnableWorldFog.Get(), void, __fastcall,
	void* ecx)
{
	if (CFG::Misc_Clean_Screenshot && I::EngineClient->IsTakingScreenshot())
		return CALL_ORIGINAL(ecx);

	if (CFG::Visuals_Remove_Fog)
		return;

	CALL_ORIGINAL(ecx);
}