#include "NetworkFix.h"

#include "../CFG.h"

MAKE_SIGNATURE(CL_ReadPackets, "engine.dll", "4C 8B DC 49 89 5B ? 55 56 57 41 54 41 55 41 56 41 57 48 83 EC ? 48 8B 05", 0x0);

MAKE_HOOK(CL_ReadPackets, Signatures::CL_ReadPackets.Get(), void, __cdecl,
	bool bFinalTick)
{
	if (!CFG::Misc_Ping_Reducer)
	{
		CALL_ORIGINAL(bFinalTick);

		return;
	}

	if (F::NetworkFix->ShouldReadPackets())
	{
		CALL_ORIGINAL(bFinalTick);
	}
}

void CReadPacketState::Store()
{
	m_flFrameTimeClientState = I::ClientState->m_frameTime;
	m_flFrameTime = I::GlobalVars->frametime;
	m_flCurTime = I::GlobalVars->curtime;
	m_nTickCount = I::GlobalVars->tickcount;
}

void CReadPacketState::Restore()
{
	I::ClientState->m_frameTime = m_flFrameTimeClientState;
	I::GlobalVars->frametime = m_flFrameTime;
	I::GlobalVars->curtime = m_flCurTime;
	I::GlobalVars->tickcount = m_nTickCount;
}

void CNetworkFix::FixInputDelay(bool bFinalTick)
{
	if (!I::EngineClient->IsInGame())
	{
		return;
	}

	if (const auto pNetChannel = I::EngineClient->GetNetChannelInfo())
	{
		if (pNetChannel->IsLoopback())
		{
			return;
		}
	}

	CReadPacketState backup = {};

	backup.Store();

	Hooks::CL_ReadPackets::Hook.Original<Hooks::CL_ReadPackets::fn>()(bFinalTick);

	m_State.Store();

	backup.Restore();
}

bool CNetworkFix::ShouldReadPackets()
{
	if (!I::EngineClient->IsInGame())
		return true;

	if (const auto pNetChannel = I::EngineClient->GetNetChannelInfo())
	{
		if (pNetChannel->IsLoopback())
			return true;
	}

	m_State.Restore();

	return false;
}
