#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CSkyboxView_Enable3dSkyboxFog, "client.dll", "40 57 48 83 EC ? E8 ? ? ? ? 48 8B F8 48 85 C0 0F 84 ? ? ? ? 48 8B 0D", 0x0);

MAKE_HOOK(CSkyboxView_Enable3dSkyboxFog, Signatures::CSkyboxView_Enable3dSkyboxFog.Get(), void, __fastcall,
	void* ecx)
{
	if (CFG::Misc_Clean_Screenshot && I::EngineClient->IsTakingScreenshot())
		return CALL_ORIGINAL(ecx);

	if (CFG::Visuals_Remove_Sky_Fog)
		return;

	CALL_ORIGINAL(ecx);
}