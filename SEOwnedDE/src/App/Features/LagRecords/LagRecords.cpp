#include "LagRecords.h"

#include <ranges>

#include "../CFG.h"

bool CLagRecords::IsSimulationTimeValid(float flCurSimTime, float flCmprSimTime)
{
	return flCurSimTime - flCmprSimTime < 0.2f;
}

void CLagRecords::AddRecord(C_TFPlayer* pPlayer)
{
	LagRecord_t newRecord = {};

	m_bSettingUpBones = true;

	const auto setup_bones_optimization{ CFG::Misc_SetupBones_Optimization };

	if (setup_bones_optimization)
	{
		pPlayer->InvalidateBoneCache();
	}

	const auto result = pPlayer->SetupBones(newRecord.BoneMatrix, 128, BONE_USED_BY_ANYTHING, I::GlobalVars->curtime);

	if (setup_bones_optimization)
	{
		auto attach = pPlayer->FirstMoveChild();
		while (attach)
		{
			if (attach->ShouldDraw())
			{
				attach->InvalidateBoneCache();
				attach->SetupBones(nullptr, -1, BONE_USED_BY_ANYTHING, I::GlobalVars->curtime);
			}

			attach = attach->NextMovePeer();
		}
	}

	m_bSettingUpBones = false;

	if (!result)
		return;

	newRecord.Player = pPlayer;
	newRecord.SimulationTime = pPlayer->m_flSimulationTime();
	newRecord.AbsOrigin = pPlayer->GetAbsOrigin();
	newRecord.VecOrigin = pPlayer->m_vecOrigin();
	newRecord.AbsAngles = pPlayer->GetAbsAngles();
	newRecord.EyeAngles = pPlayer->GetEyeAngles();
	newRecord.Velocity = pPlayer->m_vecVelocity();
	newRecord.Center = pPlayer->GetCenter();
	newRecord.Flags = pPlayer->m_fFlags();

	if (const auto pAnimState = pPlayer->GetAnimState())
		newRecord.FeetYaw = pAnimState->m_flCurrentFeetYaw;

	m_LagRecords[pPlayer].emplace_front(newRecord);
}

const LagRecord_t* CLagRecords::GetRecord(C_TFPlayer* pPlayer, int nRecord, bool bSafe)
{
	if (!bSafe)
	{
		if (!m_LagRecords.contains(pPlayer))
			return nullptr;

		if (nRecord < 0 || nRecord > static_cast<int>(m_LagRecords[pPlayer].size() - 1))
			return nullptr;
	}

	return &m_LagRecords[pPlayer][nRecord];
}

bool CLagRecords::HasRecords(C_TFPlayer* pPlayer, int* pTotalRecords)
{
	if (m_LagRecords.contains(pPlayer))
	{
		const size_t nSize = m_LagRecords[pPlayer].size();

		if (nSize <= 0)
			return false;

		if (pTotalRecords)
			*pTotalRecords = static_cast<int>(nSize - 1);

		return true;
	}

	return false;
}

void CLagRecords::UpdateRecords()
{
	const auto pLocal = H::Entities->GetLocal();

	if (!pLocal || pLocal->deadflag() || pLocal->InCond(TF_COND_HALLOWEEN_GHOST_MODE) || pLocal->InCond(TF_COND_HALLOWEEN_KART))
	{
		if (!m_LagRecords.empty())
		{
			m_LagRecords.clear();
		}

		return;
	}

	// Remove invalid players
	for (const auto pEntity : H::Entities->GetGroup(CFG::Misc_SetupBones_Optimization ? EEntGroup::PLAYERS_ALL : EEntGroup::PLAYERS_ENEMIES))
	{
		if (!pEntity || pEntity == pLocal)
		{
			continue;
		}

		const auto pPlayer = pEntity->As<C_TFPlayer>();

		if (pPlayer->deadflag())
		{
			m_LagRecords[pPlayer].clear();
		}
	}

	// Remove invalid records
	for (auto& records : m_LagRecords | std::views::values)
	{
		for (auto it = records.begin(); it != records.end(); )
		{
			const auto& curRecord = *it;
			if (!curRecord.Player || !IsSimulationTimeValid(curRecord.Player->m_flSimulationTime(), curRecord.SimulationTime))
			{
				it = records.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

bool CLagRecords::DiffersFromCurrent(const LagRecord_t* pRecord)
{
	const auto pPlayer = pRecord->Player;

	if (!pPlayer)
		return false;

	if (static_cast<int>((pPlayer->m_vecOrigin() - pRecord->AbsOrigin).Length()) != 0)
		return true;

	if (static_cast<int>((pPlayer->GetEyeAngles() - pRecord->EyeAngles).Length()) != 0)
		return true;

	if (pPlayer->m_fFlags() != pRecord->Flags)
		return true;

	if (const auto pAnimState = pPlayer->GetAnimState())
	{
		if (fabsf(pAnimState->m_flCurrentFeetYaw - pRecord->FeetYaw) > 0.0f)
			return true;
	}

	return false;
}

float CLagRecordMatrixHelper::GetLerp()
{
	if (!CFG::FakeLatency)
		return G::Lerp;
	float m_flMaxUnlag = 1.f;
	return std::clamp(CFG::FakeLatency / 1000.f, G::Lerp, m_flMaxUnlag);
}

void CLagRecordMatrixHelper::SendLerp()
{
	auto pNetChan = reinterpret_cast<CNetChannel*>(I::EngineClient->GetNetChannelInfo());
	if (!pNetChan || !I::EngineClient->IsConnected())
		return;

	float m_flWishInterp = 0.015f;
	static Timer interpTimer{};
	if (interpTimer.Run(100))
	{
		float flTarget = GetLerp();
		if (flTarget == m_flWishInterp)
			return;

		NET_SetConVar cl_interp("cl_interp", std::to_string(m_flWishInterp = flTarget).c_str());
		pNetChan->SendNetMsg(cl_interp);
		NET_SetConVar cl_interp_ratio("cl_interp_ratio", "1.0");
		pNetChan->SendNetMsg(cl_interp_ratio);
		NET_SetConVar cl_interpolate("cl_interpolate", "1");
		pNetChan->SendNetMsg(cl_interpolate);
	}
}

float GetFake()
{
	if (CFG::FakeLatency)
		return 0.f;
	
	float m_flMaxUnlag = 1.f;

	return std::clamp(CFG::FakeLatencyTime / 1000.f, 0.f, m_flMaxUnlag);
}

void CLagRecordMatrixHelper::AdjustPing(CNetChannel* pNetChan)
{
	int m_nOldInSequenceNr = pNetChan->m_nInSequenceNr, m_nOldInReliableState = pNetChan->m_nInReliableState;

	auto Set = [&]()
		{
			if (CFG::FakeLatency || !CFG::FakeLatencyTime)
				return 0.f;

			const auto pLocal = H::Entities->GetLocal();
			if (!pLocal || !pLocal->m_iClass()) {
				return 0.0f;  // or any other appropriate float value
			}

			static auto host_timescale = I::CVar->FindVar("host_timescale");
			float flTimescale = host_timescale ? host_timescale->GetFloat() : 1.f;

			int m_nOldTickBase = 0;
			static float flStaticReal = 0.f;
			float flFake = GetFake(), flReal = TICKS_TO_TIME(pLocal->m_nTickBase() - m_nOldTickBase);
			flStaticReal += (flReal + 5 * TICK_INTERVAL - flStaticReal) * 0.1f;
			std::deque<CIncomingSequence> m_dSequences;
			int nInReliableState = pNetChan->m_nInReliableState, nInSequenceNr = pNetChan->m_nInSequenceNr; float flLatency = 0.f;
			for (auto& cSequence : m_dSequences)
			{
				nInReliableState = cSequence.m_nInReliableState;
				nInSequenceNr = cSequence.m_nSequenceNr;
				flLatency = (I::GlobalVars->realtime - cSequence.m_flTime) * flTimescale - TICK_INTERVAL;
				int m_nLastInSequenceNr = 0; 	float m_flMaxUnlag = 1.f;
				if (flLatency > flFake || m_nLastInSequenceNr >= cSequence.m_nSequenceNr || flLatency > m_flMaxUnlag - flStaticReal)
					break;
			}

			pNetChan->m_nInReliableState = nInReliableState;
			pNetChan->m_nInSequenceNr = nInSequenceNr;
			return flLatency;
		};

	auto flLatency = Set();
	int m_nLastInSequenceNr = pNetChan->m_nInSequenceNr; 	float m_flFakeLatency = 0.f;

	if (CFG::FakeLatency && CFG::FakeLatencyTime || m_flFakeLatency)
	{
		m_flFakeLatency = std::clamp(m_flFakeLatency + (flLatency - m_flFakeLatency) * 0.1f, m_flFakeLatency - TICK_INTERVAL, m_flFakeLatency + TICK_INTERVAL);
		if (!flLatency && m_flFakeLatency < TICK_INTERVAL)
			m_flFakeLatency = 0.f;
	}
}

void CLagRecordMatrixHelper::RestorePing(CNetChannel* pNetChan)
{
	int m_iLastInSequence = 0;
	int m_nOldInSequenceNr = 0;
	int m_nOldInReliableState = 0;
	int m_nLastInSequenceNr = 0;
	int m_nOldTickBase = 0;
	pNetChan->m_nInSequenceNr = m_nOldInSequenceNr, pNetChan->m_nInReliableState = m_nOldInReliableState;
}

void CLagRecordMatrixHelper::Set(const LagRecord_t* pRecord)
{
	if (!pRecord)
		return;

	const auto pPlayer = pRecord->Player;

	if (!pPlayer || pPlayer->deadflag())
		return;

	const auto pCachedBoneData = pPlayer->GetCachedBoneData();

	if (!pCachedBoneData)
		return;

	m_pPlayer = pPlayer;
	m_vAbsOrigin = pPlayer->GetAbsOrigin();
	m_vAbsAngles = pPlayer->GetAbsAngles();
	memcpy(m_BoneMatrix, pCachedBoneData->Base(), sizeof(matrix3x4_t) * pCachedBoneData->Count());

	memcpy(pCachedBoneData->Base(), pRecord->BoneMatrix, sizeof(matrix3x4_t) * pCachedBoneData->Count());

	pPlayer->SetAbsOrigin(pRecord->AbsOrigin);
	pPlayer->SetAbsAngles(pRecord->AbsAngles);

	m_bSuccessfullyStored = true;
}

void CLagRecordMatrixHelper::Restore()
{
	if (!m_bSuccessfullyStored || !m_pPlayer)
		return;

	const auto pCachedBoneData = m_pPlayer->GetCachedBoneData();

	if (!pCachedBoneData)
		return;

	m_pPlayer->SetAbsOrigin(m_vAbsOrigin);
	m_pPlayer->SetAbsAngles(m_vAbsAngles);
	memcpy(pCachedBoneData->Base(), m_BoneMatrix, sizeof(matrix3x4_t) * pCachedBoneData->Count());

	m_pPlayer = nullptr;
	m_vAbsOrigin = {};
	m_vAbsAngles = {};
	std::memset(m_BoneMatrix, 0, sizeof(matrix3x4_t) * 128);
	m_bSuccessfullyStored = false;
}

Timer::Timer() {
}


bool Timer::Check(unsigned ms) const {
	return false;  
}

bool Timer::Run(unsigned ms) {
	return false;  
}

inline void Timer::Update() {

}



void CLagRecordMatrixHelper::Run(CUserCmd* pCmd)
{
	CLagRecordMatrixHelper::SendLerp();
}
