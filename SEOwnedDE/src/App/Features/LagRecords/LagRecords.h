#pragma once

#include "../../../SDK/SDK.h"

struct LagRecord_t
{
	C_TFPlayer* Player = nullptr;
	matrix3x4_t BoneMatrix[128] = {};
	float SimulationTime = -1.0f;
	Vec3 AbsOrigin = {};
	Vec3 VecOrigin = {};
	Vec3 AbsAngles = {};
	Vec3 EyeAngles = {};
	Vec3 Velocity = {};
	Vec3 Center = {};
	int Flags = 0;
	float FeetYaw = 0.0f;
};

class CLagRecords
{
	std::unordered_map<C_TFPlayer*, std::deque<LagRecord_t>> m_LagRecords = {};
	bool m_bSettingUpBones = false;

	bool IsSimulationTimeValid(float flCurSimTime, float flCmprSimTime);

public:
	void AddRecord(C_TFPlayer* pPlayer);
	const LagRecord_t* GetRecord(C_TFPlayer* pPlayer, int nRecord, bool bSafe = false);
	bool HasRecords(C_TFPlayer* pPlayer, int* pTotalRecords = nullptr);
	void UpdateRecords();
	bool DiffersFromCurrent(const LagRecord_t* pRecord);
	bool IsSettingUpBones() { return m_bSettingUpBones; }
};

MAKE_SINGLETON_SCOPED(CLagRecords, LagRecords, F);

<<<<<<< HEAD
class CIncomingSequence
{
public:
	int m_nInReliableState;
	int m_nSequenceNr;
	float m_flTime;

	CIncomingSequence(int iState, int iSequence, float flTime)
	{
		m_nInReliableState = iState;
		m_nSequenceNr = iSequence;
		m_flTime = flTime;
	}
};

class Timer
{
private:
	std::chrono::steady_clock::time_point Last;

public:
	Timer();
	bool Check(unsigned ms) const;
	bool Run(unsigned ms);
	inline void Update();
};

=======
>>>>>>> parent of eacaef0 (w)
class CLagRecordMatrixHelper
{
	C_TFPlayer* m_pPlayer = nullptr;
	Vec3 m_vAbsOrigin = {};
	Vec3 m_vAbsAngles = {};
	matrix3x4_t m_BoneMatrix[128] = {};

	bool m_bSuccessfullyStored = false;

public:
	void Set(const LagRecord_t* pRecord);
	void Restore();
<<<<<<< HEAD
	float GetLerp();
	void Run(CUserCmd* pCmd);
	void SendLerp();
	void AdjustPing(CNetChannel* pNetChan);
	void RestorePing(CNetChannel* pNetChan);
=======
>>>>>>> parent of eacaef0 (w)
};

MAKE_SINGLETON_SCOPED(CLagRecordMatrixHelper, LagRecordMatrixHelper, F);
