#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(DoEnginePostProcessing, "client.dll", "48 8B C4 44 89 48 ? 44 89 40 ? 89 50 ? 89 48", 0x0);

MAKE_HOOK(DoEnginePostProcessing, Signatures::DoEnginePostProcessing.Get(), void, __cdecl,
	int x, int y, int w, int h, bool bFlashlightIsOn, bool bPostVGui)
{
	if (CFG::Visuals_Disable_Post_Processing)
		return;

	CALL_ORIGINAL(x, y, w, h, bFlashlightIsOn, bPostVGui);
}
