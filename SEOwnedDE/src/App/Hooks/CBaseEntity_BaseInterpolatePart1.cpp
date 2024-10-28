#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CBaseEntity_BaseInterpolatePart1, "client.dll", "48 89 5C 24 ? 56 57 41 55 41 56 41 57 48 83 EC ? 4C 8B BC 24", 0x0);

MAKE_HOOK(CBaseEntity_BaseInterpolatePart1, Signatures::CBaseEntity_BaseInterpolatePart1.Get(), int, __fastcall,
	void* ecx, float& currentTime, Vector& oldOrigin, QAngle& oldAngles, Vector& oldVel, int& bNoMoreChanges)
{
	auto shouldDisableInterp = [&]
	{
		if (!CFG::Misc_Accuracy_Improvements)
		{
			const auto pLocal = H::Entities->GetLocal();

			if (!pLocal)
				return false;

			const auto pEntity = static_cast<C_BaseEntity*>(ecx);

			if (!pEntity)
				return false;

			if (pEntity == pLocal)
				return Shifting::bRecharging;

			return false;
		}

		const auto pLocal = H::Entities->GetLocal();

		if (!pLocal)
			return false;

		const auto pEntity = static_cast<C_BaseEntity*>(ecx);

		if (!pEntity)
			return false;

		if (pEntity == pLocal)
			return Shifting::bRecharging;

		if (pEntity->GetClassId() == ETFClassIds::CTFPlayer)
			return pEntity != pLocal;

		if (pEntity->GetClassId() == ETFClassIds::CBaseDoor)
			return true;

		return false;
	};

	if (shouldDisableInterp())
	{
		bNoMoreChanges = 1;
		return 0;
	}

	return CALL_ORIGINAL(ecx, currentTime, oldOrigin, oldAngles, oldVel, bNoMoreChanges);
}
