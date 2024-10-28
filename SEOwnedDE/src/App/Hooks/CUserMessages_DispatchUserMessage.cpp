#include "../../SDK/SDK.h"

#include "../Features/CFG.h"
#include "../Features/SeedPred/SeedPred.h"

MAKE_SIGNATURE(CUserMessages_DispatchUserMessage, "client.dll", "40 56 48 83 EC ? 49 8B F0", 0x0);

MAKE_HOOK(CUserMessages_DispatchUserMessage, Signatures::CUserMessages_DispatchUserMessage.Get(), bool, __fastcall,
	void* ecx, int msg_type, bf_read& msg_data)
{
	if (msg_type == 10 && CFG::Visuals_Remove_Screen_Shake)
	{
		return true;
	}

	if (msg_type == 11 && CFG::Visuals_Remove_Screen_Fade)
	{
		return true;
	}

	if (msg_type == 5 && F::SeedPred->ParsePlayerPerf(msg_data))
	{
		return true;
	}

	return CALL_ORIGINAL(ecx, msg_type, msg_data);
}
