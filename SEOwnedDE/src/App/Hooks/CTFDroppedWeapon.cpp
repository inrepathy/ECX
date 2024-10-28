#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CTFDroppedWeapon_OnDataChanged, "client.dll", "40 55 53 41 57 48 8D 6C 24", 0x0);
MAKE_SIGNATURE(CTFDroppedWeapon_ClientThink, "client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 83 EC ? 4C 89 7C 24", 0x0);
MAKE_SIGNATURE(CTFDroppedWeapon_OnInternalDrawModel, "client.dll", "48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B F2 48 8B F9 E8 ? ? ? ? 84 C0", 0x0);

MAKE_HOOK(CTFDroppedWeapon_OnDataChanged, Signatures::CTFDroppedWeapon_OnDataChanged.Get(), void, __fastcall,
	void* ecx, DataUpdateType_t updateType)
{
	if (CFG::Visuals_Disable_Dropped_Weapons)
		return;

	CALL_ORIGINAL(ecx, updateType);
}

MAKE_HOOK(CTFDroppedWeapon_ClientThink, Signatures::CTFDroppedWeapon_ClientThink.Get(), void, __fastcall,
	void* ecx)
{
	if (CFG::Visuals_Disable_Dropped_Weapons)
		return;

	CALL_ORIGINAL(ecx);
}

MAKE_HOOK(CTFDroppedWeapon_OnInternalDrawModel, Signatures::CTFDroppedWeapon_OnInternalDrawModel.Get(), bool, __fastcall,
	void* ecx, void* pInfo)
{
	if (CFG::Visuals_Disable_Dropped_Weapons)
		return false;

	return CALL_ORIGINAL(ecx, pInfo);
}
