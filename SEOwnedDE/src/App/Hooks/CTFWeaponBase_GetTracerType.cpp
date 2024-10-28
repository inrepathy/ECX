#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CTFWeaponBase_GetTracerType, "client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 4C 8D 0D", 0x0);

MAKE_HOOK(CTFWeaponBase_GetTracerType, Signatures::CTFWeaponBase_GetTracerType.Get(), const char*, __fastcall,
	C_TFWeaponBase* ecx)
{
	if (const auto nType = CFG::Visuals_Tracer_Type)
	{
		if (const auto pLocal = H::Entities->GetLocal())
		{
			if (const auto pWeapon = ecx)
			{
				if (pWeapon->m_hOwner().Get() == pLocal)
				{
					auto getTracerName = [&](int nIndex)
					{
						switch (nIndex)
						{
							case 1: return pLocal->m_iTeamNum() == 2 ? "bullet_tracer_raygun_red" : "bullet_tracer_raygun_blue";
							case 2: return "dxhr_sniper_rail";
							case 3: return pLocal->m_iTeamNum() == 2 ? "dxhr_sniper_rail_red" : "dxhr_sniper_rail_blue";
							case 4: return pLocal->m_iTeamNum() == 2 ? "bullet_bignasty_tracer01_red" : "bullet_bignasty_tracer01_blue";
							case 5: return pLocal->m_iTeamNum() == 2 ? "dxhr_lightningball_hit_zap_red" : "dxhr_lightningball_hit_zap_blue";
							case 6: return "merasmus_zap";
							default: return CALL_ORIGINAL(ecx);
						}
					};

					const bool bIsTF2DevsHorribleCode = pWeapon->m_iItemDefinitionIndex() == Sniper_m_TheMachina
						|| pWeapon->m_iItemDefinitionIndex() == Sniper_m_ShootingStar;

					return getTracerName((nType > 6) ? SDKUtils::RandomInt(bIsTF2DevsHorribleCode ? 1 : 0, (nType == 8) ? 5 : 6) : nType);
				}
			}
		}
	}

	return CALL_ORIGINAL(ecx);
}
