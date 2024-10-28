#include "../../SDK/SDK.h"

MAKE_SIGNATURE(CTFWeapon_ShouldDraw, "client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 4C 8D 79", 0x0);

MAKE_HOOK(CTFWeapon_ShouldDraw, Signatures::CTFWeapon_ShouldDraw.Get(), bool, __fastcall,
	void* ecx)
{
	if (const auto weapon = reinterpret_cast<C_TFWeaponBase*>(reinterpret_cast<uintptr_t>(ecx) - sizeof(uintptr_t)))
	{
		if (weapon->GetWeaponID() == TF_WEAPON_MEDIGUN)
		{
			if (const auto owner = reinterpret_cast<C_TFPlayer*>(weapon->m_hOwner().Get()))
			{
				if (owner->m_hActiveWeapon().Get() == weapon && !owner->InCond(TF_COND_TAUNTING))
				{
					return true;
				}
			}
		}
	}

	return CALL_ORIGINAL(ecx);
}
