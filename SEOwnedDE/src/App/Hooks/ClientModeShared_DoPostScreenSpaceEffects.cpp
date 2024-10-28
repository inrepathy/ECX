#include "../../SDK/SDK.h"

#include "../Features/CFG.h"
#include "../Features/MiscVisuals/MiscVisuals.h"

MAKE_HOOK(ClientModeShared_DoPostScreenSpaceEffects, Memory::GetVFunc(I::ClientModeShared, 39), bool, __fastcall,
	CClientModeShared* ecx, const CViewSetup* pSetup)
{
	const auto original = CALL_ORIGINAL(ecx, pSetup);

	F::MiscVisuals->SniperLines();
	F::MiscVisuals->ProjectileArc();

	return original;
}
