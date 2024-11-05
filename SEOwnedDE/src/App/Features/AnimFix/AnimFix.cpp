#include "../../SEOwnedDE/src/SDK/SDK.h"
#include "AnimFix.h"


#include "../../Features/CFG.h"

void CAnimFix::Update(CUserCmd* pCmd) {
	const auto pLocal = H::Entities->GetLocal();
	const auto pWeapon = H::Entities->GetWeapon();


	const auto currentanimstate = pLocal->GetAnimState();

	if (!pLocal || !pLocal->IsInValidTeam())
		return;

	/*	if ( gClientState->chokedcommands )
	{
		TicksElapsed = gClientState->chokedcommands;
		return;
	}*/ 

	if (I::ClientState->chokedcommands) {
		TicksElapsed = I::ClientState->chokedcommands;
		return;
	}


	static int OldTickCount = I::ClientState->oldtickcount;
	if (OldTickCount == I::ClientState->oldtickcount)
	{
		return;
	}

    


	if (OldTickCount != I::ClientState->oldtickcount)
	{
		OldTickCount = I::ClientState->oldtickcount;
	}



	if (pLocal->m_flSimulationTime() < pLocal->m_flOldSimulationTime())
	{
		G::WarpTimeStamps[pLocal->entindex()] = I::EngineClient->Time() + TICKS_TO_TIME(4);
	}

	if (G::WarpTimeStamps[pLocal->entindex()] > I::EngineClient->Time())
	{
		return;
	}

	int CTFPlayerAnimState = G::bUpdatingAnims;

	float OldFrameTime = I::ClientState->m_frameTime;
	int OldSequence = pLocal->m_nSequence();
	float OldCycle = pLocal->m_flCycle();
	auto OldPoseParams = pLocal->m_flPoseParameter();
	char pOldAnimState[sizeof(CTFPlayerAnimState)] = {};


	memcpy(pOldAnimState, currentanimstate, sizeof(CTFPlayerAnimState));


	auto Restore = [&]()
		{
			I::ClientState->m_frameTime = OldFrameTime;
			pLocal->m_nSequence() = OldSequence;
			pLocal->m_flCycle() = OldCycle;
			pLocal->m_flPoseParameter() = OldPoseParams;
			memcpy(currentanimstate, pOldAnimState, sizeof(CTFPlayerAnimState));
		};

	if (CFG::AntiAim && !(H::Input->IsDown(CFG::Exploits_RapidFire_Key))) {

		// pasting from ateris is weird..
		currentanimstate->m_flCurrentFeetYaw = pCmd->viewangles.y;
		currentanimstate->m_flEyeYaw = pCmd->viewangles.y;

		G::bUpdatingAnims = true;
		currentanimstate->Update(pCmd->viewangles.y, CFG::AntiAim_FakeAngle);

		/*
		if ( !gLocalPlayer->SetupBones( G->AnimationFix->FakeMatrix, 128, BONE_USED_BY_ANYTHING, gGlobals->curtime ) )
		{
			G->AnimationFix->FakeMatrixBuilt = false;
			Restore( );
			return;
		}*/

		G::FakeMatrixBuilt = true;
		Restore();

		//pCmd->viewangles.y = CFG::AntiAim_Yaw /* && CFG::AntiAim_Pitch*/; // bugs out the aa movement fix so
	}


	currentanimstate->m_flCurrentFeetYaw = pCmd->viewangles.y;
	currentanimstate->m_flEyeYaw = pCmd->viewangles.y;

	G::bUpdatingAnims = true;
	pLocal->UpdateClientSideAnimation();

	I::ClientState->m_frameTime = I::Prediction->m_bEnginePaused ? 0.0f : (I::GlobalVars->interval_per_tick * TicksElapsed);


	G::bUpdatingAnims = true;
	currentanimstate->Update(pCmd->viewangles.y, pCmd->viewangles.x);

	I::ClientState->m_frameTime = OldFrameTime;

	TicksElapsed = 1;

}