#include "../../SDK/SDK.h"

#include "../Features/Materials/Materials.h"
#include "../Features/Outlines/Outlines.h"

MAKE_SIGNATURE(CBaseWorldView_DrawExecute, "client.dll", "40 53 55 56 41 56 41 57 48 81 EC", 0x0);
MAKE_SIGNATURE(CParticleSystemMgr_DrawRenderCache, "client.dll", "48 8B C4 88 50 ? 48 89 48 ? 55 57", 0x0);

bool isDrawingWorld = false;

MAKE_HOOK(CBaseWorldView_DrawExecute, Signatures::CBaseWorldView_DrawExecute.Get(), void, __fastcall,
	void* ecx, float waterHeight, view_id_t viewID, float waterZAdjust)
{
	isDrawingWorld = true;
	CALL_ORIGINAL(ecx, waterHeight, viewID, waterZAdjust);
	isDrawingWorld = false;
}

MAKE_HOOK(CParticleSystemMgr_DrawRenderCache, Signatures::CParticleSystemMgr_DrawRenderCache.Get(), void, __fastcall,
	void* ecx, bool bShadowDepth)
{
	if (isDrawingWorld)
	{
		if (const auto rc = I::MaterialSystem->GetRenderContext())
		{
			rc->ClearBuffers(false, false, true);
		}

		F::Materials->Run();
		F::Outlines->RunModels();
	}

	CALL_ORIGINAL(ecx, bShadowDepth);
}
