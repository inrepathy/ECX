#include "../../SDK/SDK.h"

#include "../Features/Materials/Materials.h"
#include "../Features/Outlines/Outlines.h"
#include "../Features/Paint/Paint.h"

MAKE_HOOK(ISurface_OnScreenSizeChanged, Memory::GetVFunc(I::MatSystemSurface, 111), void, __fastcall,
	ISurface* ecx, int nOldWidth, int OldHeight)
{
	CALL_ORIGINAL(ecx, nOldWidth, OldHeight);

	H::Fonts->Reload();
	gDraw().UpdateScreenSize();

	F::Materials->CleanUp();
	F::Outlines->CleanUp();
	F::Paint->CleanUp();
}
