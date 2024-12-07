#include "../../SDK/SDK.h"

#include "../Features/CFG.h"
#include "../Features/Outlines/Outlines.h"
#include "../Features/Paint/Paint.h"

MAKE_SIGNATURE(CViewRender_DrawViewModels, "client.dll", "48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 8B FA", 0x0);

MAKE_HOOK(CViewRender_DrawViewModels, Signatures::CViewRender_DrawViewModels.Get(), void, __fastcall,
	void* ecx, const CViewSetup& viewRender, bool drawViewmodel)
{
	CALL_ORIGINAL(ecx, viewRender, drawViewmodel);

	F::Outlines->Run();
	F::Paint->Run();
}
