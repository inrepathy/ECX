#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CTFViewModel_CalcViewModelView, "client.dll", "48 89 74 24 ? 55 41 56 41 57 48 8D AC 24", 0x0);

MAKE_HOOK(CTFViewModel_CalcViewModelView, Signatures::CTFViewModel_CalcViewModelView.Get(), void, __fastcall,
	void* ecx, C_BasePlayer* owner, const Vector &eyePosition, const QAngle &eyeAngles)
{
	const auto pLocal = H::Entities->GetLocal();

	if (pLocal && !pLocal->deadflag() && CFG::Visuals_ViewModel_Active)
	{
		Vec3 vForward = {}, vRight = {}, vUp = {};
		Math::AngleVectors(eyeAngles, &vForward, &vRight, &vUp);

		const Vec3 vEyePosition = eyePosition + (
			(vForward * CFG::Visuals_ViewModel_Offset_Forward) +
			(vRight * CFG::Visuals_ViewModel_Offset_Right) +
			(vUp * CFG::Visuals_ViewModel_Offset_Up)
		);

		CALL_ORIGINAL(ecx, owner, vEyePosition, eyeAngles);
	}
	else
	{
		CALL_ORIGINAL(ecx, owner, eyePosition, eyeAngles);
	}
}
