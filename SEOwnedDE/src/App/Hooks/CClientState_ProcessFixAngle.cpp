#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CClientState_ProcessFixAngle, "engine.dll", "40 53 48 83 EC ? F3 0F 10 42", 0x0);

MAKE_HOOK(CClientState_ProcessFixAngle, Signatures::CClientState_ProcessFixAngle.Get(), bool, __fastcall,
	CClientState* ecx, void* msg)
{
	if (CFG::Misc_Prevent_Server_Angle_Change)
	{
		return true;
	}

	return CALL_ORIGINAL(ecx, msg);
}
