#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CTFPlayer_AvoidPlayers, "client.dll", "48 89 54 24 ? 55 41 56", 0x0);

MAKE_HOOK(CTFPlayer_AvoidPlayers, Signatures::CTFPlayer_AvoidPlayers.Get(), void, __fastcall,
	void* ecx, CUserCmd* pCmd)
{
	if (CFG::Misc_No_Push)
		return;

	CALL_ORIGINAL(ecx, pCmd);
}
