#include "../../SDK/SDK.h"

MAKE_SIGNATURE(CBasePlayer_PostDataUpdate_SetAbsVelocityCall, "client.dll", "E8 ? ? ? ? 0F 28 74 24 ? 8B D6", 0x5);

MAKE_HOOK(CBaseEntity_SetAbsVelocity, Signatures::CBaseEntity_SetAbsVelocity.Get(), void, __fastcall,
	C_BasePlayer* ecx, const Vector& vecAbsVelocity)
{
	if (reinterpret_cast<std::uintptr_t>(_ReturnAddress()) == Signatures::CBasePlayer_PostDataUpdate_SetAbsVelocityCall.Get())
	{
		if (const auto pBasePlayer = ecx->As<C_TFPlayer>())
		{
			if (G::mapVelFixRecords.contains(pBasePlayer))
			{
				const auto& record = G::mapVelFixRecords[pBasePlayer];
				const float flSimTimeDelta = pBasePlayer->m_flSimulationTime() - record.m_flSimulationTime;

				if (flSimTimeDelta > 0.0f)
				{
					Vec3 vOldOrigin = record.m_vecOrigin;

					const int nCurFlags = pBasePlayer->m_fFlags();
					const int nOldFlags = record.m_fFlags;

					if (!(nCurFlags & FL_ONGROUND) && !(nOldFlags & FL_ONGROUND))
					{
						bool bCorrected = false;

						if ((nCurFlags & FL_DUCKING) && !(nOldFlags & FL_DUCKING))
						{
							vOldOrigin.z += 20.0f;
							bCorrected = true;
						}

						if (!(nCurFlags & FL_DUCKING) && (nOldFlags & FL_DUCKING))
						{
							vOldOrigin.z -= 20.0f;
							bCorrected = true;
						}

						if (bCorrected)
						{
							Vec3 vNewVelocity = vecAbsVelocity;
							vNewVelocity.z = (pBasePlayer->m_vecOrigin().z - vOldOrigin.z) / flSimTimeDelta;
							CALL_ORIGINAL(ecx, vNewVelocity);
							return;
						}
					}
				}
			}
		}
	}

	CALL_ORIGINAL(ecx, vecAbsVelocity);
}
