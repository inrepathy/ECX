#include "../../SDK/SDK.h"

MAKE_SIGNATURE(INetChannel_SendNetMsg, "engine.dll", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 48 8B F1 45 0F B6 F1", 0x0);
MAKE_SIGNATURE(WriteUsercmd, "client.dll", "40 56 57 48 83 EC ? 41 8B 40", 0x0);

//credits: KGB

bool WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to)
{
	CUserCmd nullcmd = {};
	CUserCmd* f = nullptr;

	if (from == -1)
	{
		f = &nullcmd;
	}
	else
	{
		f = I::Input->GetUserCmd(from);

		if (!f)
		{
			f = &nullcmd;
		}
	}

	CUserCmd* t = I::Input->GetUserCmd(to);

	if (!t)
	{
		t = &nullcmd;
	}

	reinterpret_cast<void(__cdecl *)(bf_write*, CUserCmd*, CUserCmd*)>(Signatures::WriteUsercmd.Get())(buf, t, f);

	return !buf->m_bOverflow;
}

MAKE_HOOK(INetChannel_SendNetMsg, Signatures::INetChannel_SendNetMsg.Get(), bool, __fastcall,
	CNetChannel* pNet, INetMessage& msg, bool bForceReliable, bool bVoice)
{
	if (msg.GetType() == clc_Move)
	{
		const auto pMsg = reinterpret_cast<CLC_Move*>(&msg);

		const int nLastOutGoingCommand = I::ClientState->lastoutgoingcommand;
		const int nChokedCommands = I::ClientState->chokedcommands;
		const int nNextCommandNr = nLastOutGoingCommand + nChokedCommands + 1;

		unsigned char data[4000] = {};
		pMsg->m_DataOut.StartWriting(data, sizeof(data));

		pMsg->m_nNewCommands = 1 + nChokedCommands;
		pMsg->m_nNewCommands = std::clamp(pMsg->m_nNewCommands, 0, MAX_NEW_COMMANDS);

		const int nExtraCommands = nChokedCommands + 1 - pMsg->m_nNewCommands;
		const int nCmdBackup = std::max(2, nExtraCommands);

		pMsg->m_nBackupCommands = std::clamp(nCmdBackup, 0, MAX_BACKUP_COMMANDS);

		const int nNumCmds = pMsg->m_nNewCommands + pMsg->m_nBackupCommands;
		int nFrom = -1;

		bool bOk = true;

		for (int nTo = nNextCommandNr - nNumCmds + 1; nTo <= nNextCommandNr; nTo++)
		{
			bOk = bOk && WriteUsercmdDeltaToBuffer(&pMsg->m_DataOut, nFrom, nTo);
			nFrom = nTo;
		}

		if (bOk)
		{
			if (nExtraCommands > 0)
			{
				pNet->m_nChokedPackets -= nExtraCommands;
			}

			CALL_ORIGINAL(pNet, reinterpret_cast<INetMessage&>(*pMsg), bForceReliable, bVoice);
		}

		return true;
	}

	return CALL_ORIGINAL(pNet, msg, bForceReliable, bVoice);
}
