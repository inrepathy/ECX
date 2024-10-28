#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CTFWeaponBase_GetShootSound, "client.dll", "40 55 56 41 56 48 83 EC ? 80 B9", 0x0);

MAKE_HOOK(
	CTFWeaponBase_GetShootSound, Signatures::CTFWeaponBase_GetShootSound.Get(), const char*, __fastcall,
	C_TFWeaponBase* ecx, int iIndex)
{
	if (CFG::Misc_MVM_Giant_Weapon_Sounds)
	{
		if (const auto pWeapon = ecx)
		{
			if (const auto pLocal = H::Entities->GetLocal())
			{
				if (pWeapon->m_hOwner().Get() == pLocal)
				{
					const int nOldTeam = pWeapon->m_iTeamNum();
					pWeapon->m_iTeamNum() = TF_TEAM_PVE_INVADERS_GIANTS;
					const auto ret = CALL_ORIGINAL(ecx, iIndex);
					pWeapon->m_iTeamNum() = nOldTeam;

					//credits: KGB

					static auto fireHash = HASH_CT("Weapon_FlameThrower.Fire");

					if (HASH_RT(ret) == fireHash)
						return "MVM.GiantPyro_FlameStart";

					static auto fireLoopHash = HASH_CT("Weapon_FlameThrower.FireLoop");

					if (HASH_RT(ret) == fireLoopHash)
						return "MVM.GiantPyro_FlameLoop";

					static auto grenadeHash = HASH_CT("Weapon_GrenadeLauncher.Single");

					if (HASH_RT(ret) == grenadeHash)
						return "MVM.GiantDemoman_Grenadeshoot";

					return ret;
				}
			}
		}
	}

	return CALL_ORIGINAL(ecx, iIndex);
}
