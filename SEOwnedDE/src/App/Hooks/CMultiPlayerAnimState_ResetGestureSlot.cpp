#include "../../SDK/SDK.h"

MAKE_SIGNATURE(CMultiPlayerAnimState_ResetGestureSlot, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? 8B DA 48 8B F9 E8 ? ? ? ? 84 C0 74", 0x0);

MAKE_HOOK(CMultiPlayerAnimState_ResetGestureSlot, Signatures::CMultiPlayerAnimState_ResetGestureSlot.Get(), void, __fastcall,
	CMultiPlayerAnimState* ecx, int iGestureSlot)
{
	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (const auto pAnimState = ecx)
		{
			if (pAnimState->m_pEntity == pLocal && iGestureSlot == GESTURE_SLOT_VCD && G::bStartedFakeTaunt)
			{
				return;
			}
		}
	}

	CALL_ORIGINAL(ecx, iGestureSlot);
}
