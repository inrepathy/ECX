#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CViewRender_PerformScreenOverlay, "client.dll", "4C 8B DC 49 89 5B ? 89 54 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 81 EC", 0x0);

MAKE_HOOK(CViewRender_PerformScreenOverlay, Signatures::CViewRender_PerformScreenOverlay.Get(), void, __fastcall,
	void* ecx, int x, int y, int w, int h)
{
	if (CFG::Visuals_Remove_Screen_Overlay)
	{
		auto shouldDoOverrides = [&]()
		{
			if (CFG::Visuals_Removals_Mode == 1)
			{
				if (const auto pLocal = H::Entities->GetLocal())
				{
					if (pLocal->m_iObserverMode() == OBS_MODE_IN_EYE)
					{
						return false;
					}
				}
			}

			return true;
		};

		if (shouldDoOverrides())
		{
			return;
		}
	}

	CALL_ORIGINAL(ecx, x, y, w, h);
}