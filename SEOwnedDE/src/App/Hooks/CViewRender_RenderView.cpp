#include "../../SDK/SDK.h"

#include "../Features/SpyCamera/SpyCamera.h"
#include "../Features/CFG.h"

MAKE_HOOK(CViewRender_RenderView, Memory::GetVFunc(I::ViewRender, 6), void, __fastcall,
	void* ecx, const CViewSetup& view, int nClearFlags, int whatToDraw)
{
	CALL_ORIGINAL(ecx, view, nClearFlags, whatToDraw);

	F::SpyCamera->UpdateViewSetup(view);
}
