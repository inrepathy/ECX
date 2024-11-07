#include "MovementSimulation.h"

#include "../LagRecords/LagRecords.h"

#include "../CFG.h"

void CMovementSimulation::CPlayerDataBackup::Store(C_TFPlayer* pPlayer)
{
	m_vecOrigin = pPlayer->m_vecOrigin();
	m_vecVelocity = pPlayer->m_vecVelocity();
	m_vecBaseVelocity = pPlayer->m_vecBaseVelocity();
	m_vecViewOffset = pPlayer->m_vecViewOffset();
	m_hGroundEntity = pPlayer->m_hGroundEntity();
	m_fFlags = pPlayer->m_fFlags();
	m_flDucktime = pPlayer->m_flDucktime();
	m_flDuckJumpTime = pPlayer->m_flDuckJumpTime();
	m_bDucked = pPlayer->m_bDucked();
	m_bDucking = pPlayer->m_bDucking();
	m_bInDuckJump = pPlayer->m_bInDuckJump();
	m_flModelScale = pPlayer->m_flModelScale();
	m_nButtons = pPlayer->m_nButtons();
	m_flLastMovementStunChange = pPlayer->m_flLastMovementStunChange();
	m_flStunLerpTarget = pPlayer->m_flStunLerpTarget();
	m_bStunNeedsFadeOut = pPlayer->m_bStunNeedsFadeOut();
	m_flPrevTauntYaw = pPlayer->m_flPrevTauntYaw();
	m_flTauntYaw = pPlayer->m_flTauntYaw();
	m_flCurrentTauntMoveSpeed = pPlayer->m_flCurrentTauntMoveSpeed();
	m_iKartState = pPlayer->m_iKartState();
	m_flVehicleReverseTime = pPlayer->m_flVehicleReverseTime();
	m_flHypeMeter = pPlayer->m_flHypeMeter();
	m_flMaxspeed = pPlayer->m_flMaxspeed();
	m_nAirDucked = pPlayer->m_nAirDucked();
	m_bJumping = pPlayer->m_bJumping();
	m_iAirDash = pPlayer->m_iAirDash();
	m_flWaterJumpTime = pPlayer->m_flWaterJumpTime();
	m_flSwimSoundTime = pPlayer->m_flSwimSoundTime();
	m_surfaceProps = pPlayer->m_surfaceProps();
	m_pSurfaceData = pPlayer->m_pSurfaceData();
	m_surfaceFriction = pPlayer->m_surfaceFriction();
	m_chTextureType = pPlayer->m_chTextureType();
	m_vecPunchAngle = pPlayer->m_vecPunchAngle();
	m_vecPunchAngleVel = pPlayer->m_vecPunchAngleVel();
	m_MoveType = pPlayer->m_MoveType();
	m_MoveCollide = pPlayer->m_MoveCollide();
	m_vecLadderNormal = pPlayer->m_vecLadderNormal();
	m_flGravity = pPlayer->m_flGravity();
	m_nWaterLevel = pPlayer->m_nWaterLevel_C_BaseEntity();
	m_nWaterType = pPlayer->m_nWaterType();
	m_flFallVelocity = pPlayer->m_flFallVelocity();
	m_nPlayerCond = pPlayer->m_nPlayerCond();
	m_nPlayerCondEx = pPlayer->m_nPlayerCondEx();
	m_nPlayerCondEx2 = pPlayer->m_nPlayerCondEx2();
	m_nPlayerCondEx3 = pPlayer->m_nPlayerCondEx3();
	m_nPlayerCondEx4 = pPlayer->m_nPlayerCondEx4();
	_condition_bits = pPlayer->_condition_bits();
}

void CMovementSimulation::CPlayerDataBackup::Restore(C_TFPlayer* pPlayer)
{
	pPlayer->m_vecOrigin() = m_vecOrigin;
	pPlayer->m_vecVelocity() = m_vecVelocity;
	pPlayer->m_vecBaseVelocity() = m_vecBaseVelocity;
	pPlayer->m_vecViewOffset() = m_vecViewOffset;
	pPlayer->m_hGroundEntity() = m_hGroundEntity;
	pPlayer->m_fFlags() = m_fFlags;
	pPlayer->m_flDucktime() = m_flDucktime;
	pPlayer->m_flDuckJumpTime() = m_flDuckJumpTime;
	pPlayer->m_bDucked() = m_bDucked;
	pPlayer->m_bDucking() = m_bDucking;
	pPlayer->m_bInDuckJump() = m_bInDuckJump;
	pPlayer->m_flModelScale() = m_flModelScale;
	pPlayer->m_nButtons() = m_nButtons;
	pPlayer->m_flLastMovementStunChange() = m_flLastMovementStunChange;
	pPlayer->m_flStunLerpTarget() = m_flStunLerpTarget;
	pPlayer->m_bStunNeedsFadeOut() = m_bStunNeedsFadeOut;
	pPlayer->m_flPrevTauntYaw() = m_flPrevTauntYaw;
	pPlayer->m_flTauntYaw() = m_flTauntYaw;
	pPlayer->m_flCurrentTauntMoveSpeed() = m_flCurrentTauntMoveSpeed;
	pPlayer->m_iKartState() = m_iKartState;
	pPlayer->m_flVehicleReverseTime() = m_flVehicleReverseTime;
	pPlayer->m_flHypeMeter() = m_flHypeMeter;
	pPlayer->m_flMaxspeed() = m_flMaxspeed;
	pPlayer->m_nAirDucked() = m_nAirDucked;
	pPlayer->m_bJumping() = m_bJumping;
	pPlayer->m_iAirDash() = m_iAirDash;
	pPlayer->m_flWaterJumpTime() = m_flWaterJumpTime;
	pPlayer->m_flSwimSoundTime() = m_flSwimSoundTime;
	pPlayer->m_surfaceProps() = m_surfaceProps;
	pPlayer->m_pSurfaceData() = m_pSurfaceData;
	pPlayer->m_surfaceFriction() = m_surfaceFriction;
	pPlayer->m_chTextureType() = m_chTextureType;
	pPlayer->m_vecPunchAngle() = m_vecPunchAngle;
	pPlayer->m_vecPunchAngleVel() = m_vecPunchAngleVel;
	pPlayer->m_flJumpTime() = m_flJumpTime;
	pPlayer->m_MoveType() = m_MoveType;
	pPlayer->m_MoveCollide() = m_MoveCollide;
	pPlayer->m_vecLadderNormal() = m_vecLadderNormal;
	pPlayer->m_flGravity() = m_flGravity;
	pPlayer->m_nWaterLevel_C_BaseEntity() = m_nWaterLevel;
	pPlayer->m_nWaterType() = m_nWaterType;
	pPlayer->m_flFallVelocity() = m_flFallVelocity;
	pPlayer->m_nPlayerCond() = m_nPlayerCond;
	pPlayer->m_nPlayerCondEx() = m_nPlayerCondEx;
	pPlayer->m_nPlayerCondEx2() = m_nPlayerCondEx2;
	pPlayer->m_nPlayerCondEx3() = m_nPlayerCondEx3;
	pPlayer->m_nPlayerCondEx4() = m_nPlayerCondEx4;
	pPlayer->_condition_bits() = _condition_bits;
}

void CMovementSimulation::SetupMoveData(C_TFPlayer* pPlayer, CMoveData* pMoveData)
{
	if (!pPlayer || !pMoveData)
		return;

	pMoveData->m_bFirstRunOfFunctions = false;
	pMoveData->m_bGameCodeMovedPlayer = false;
	pMoveData->m_nPlayerHandle = pPlayer->GetRefEHandle();
	pMoveData->m_vecVelocity = pPlayer->m_vecVelocity();
	pMoveData->m_vecAbsOrigin = pPlayer->m_vecOrigin();
	pMoveData->m_flMaxSpeed = pPlayer->TeamFortress_CalculateMaxSpeed();

	if (m_PlayerDataBackup.m_fFlags & FL_DUCKING)
		pMoveData->m_flMaxSpeed *= 0.3333f;

	pMoveData->m_flClientMaxSpeed = pMoveData->m_flMaxSpeed;

	pMoveData->m_vecViewAngles = { 0.0f, Math::VelocityToAngles(pMoveData->m_vecVelocity).y, 0.0f };

	if (CFG::Aimbot_Projectile_Aim_Prediction_Method == 0)
	{


		pMoveData->m_flForwardMove = 450.0f;
		pMoveData->m_flSideMove = 0.0f;
	}

	else
	{
		Vec3 vForward = {}, vRight = {};
		Math::AngleVectors(pMoveData->m_vecViewAngles, &vForward, &vRight, nullptr);

		pMoveData->m_flForwardMove = (pMoveData->m_vecVelocity.y - vRight.y / vRight.x * pMoveData->m_vecVelocity.x) / (vForward.y - vRight.y / vRight.x * vForward.x);
		pMoveData->m_flSideMove = (pMoveData->m_vecVelocity.x - vForward.x * pMoveData->m_flForwardMove) / vRight.x;
	}

	if (CFG::Aimbot_Projectile_Aim_Prediction_Method == 2)
	{
		pMoveData->m_flForwardMove = 450.0f;
		pMoveData->m_flSideMove = 0.0f;

		Math::ClampAngles(pMoveData->m_vecAngles);
	}
	
	if (CFG::Aimbot_Projectile_Aim_Prediction_Method == 3)
	{
		pMoveData->m_flForwardMove = 450.0f;
		pMoveData->m_flSideMove = 0.0f;

		pMoveData->m_vecAngles.x = Math::NormalizeAngle(pMoveData->m_vecAngles.x);
		pMoveData->m_vecAngles.y = Math::NormalizeAngle(pMoveData->m_vecAngles.y);
		pMoveData->m_vecAngles.z = Math::NormalizeAngle(pMoveData->m_vecAngles.z); // prolly dont need this

		Math::ClampAngles(pMoveData->m_vecAngles);
	}

	if (CFG::Aimbot_Projectile_Aim_Prediction_Method == 4)
	{
		pMoveData->m_flForwardMove = 450.0f;
		pMoveData->m_flSideMove = 0.0f;

		pMoveData->m_vecAngles.x = Math::NormalizeAngle(pMoveData->m_vecAngles.x);
		pMoveData->m_vecAngles.y = Math::NormalizeAngle(pMoveData->m_vecAngles.y);
		pMoveData->m_vecAngles.z = Math::NormalizeAngle(pMoveData->m_vecAngles.z); 
	}




	const float flSpeed = pPlayer->m_vecVelocity().Length2D();

	if (flSpeed <= pMoveData->m_flMaxSpeed * 0.1f)
		pMoveData->m_flForwardMove = pMoveData->m_flSideMove = 0.0f;

	pMoveData->m_vecAngles = pMoveData->m_vecViewAngles;
	pMoveData->m_vecOldAngles = pMoveData->m_vecAngles;

	if (pPlayer->m_hConstraintEntity())
		pMoveData->m_vecConstraintCenter = pPlayer->m_hConstraintEntity()->GetAbsOrigin();

	else pMoveData->m_vecConstraintCenter = pPlayer->m_vecConstraintCenter();

	pMoveData->m_flConstraintRadius = pPlayer->m_flConstraintRadius();
	pMoveData->m_flConstraintWidth = pPlayer->m_flConstraintWidth();
	pMoveData->m_flConstraintSpeedFactor = pPlayer->m_flConstraintSpeedFactor();

	m_flYawTurnRate = 0.0f;

	if (CFG::Aimbot_Projectile_Ground_Strafe_Prediction && (m_PlayerDataBackup.m_fFlags & FL_ONGROUND) && F::LagRecords->HasRecords(pPlayer))
	{
		if (m_MoveData.m_vecVelocity.Length2D() < (m_MoveData.m_flMaxSpeed * 0.85f))
		{
			return;
		}

		const auto pRecord0 = F::LagRecords->GetRecord(pPlayer, 0);
		const auto pRecord1 = F::LagRecords->GetRecord(pPlayer, 1);
		const auto pRecord2 = F::LagRecords->GetRecord(pPlayer, 2);
		const auto pRecord3 = F::LagRecords->GetRecord(pPlayer, 3);
		const auto pRecord4 = F::LagRecords->GetRecord(pPlayer, 4);

		if (pRecord0 && pRecord1 && pRecord2 && pRecord3 && pRecord4)
		{
			const float flYaw0 = Math::VelocityToAngles(pRecord0->Velocity).y;
			const float flYaw1 = Math::VelocityToAngles(pRecord1->Velocity).y;
			const float flYaw2 = Math::VelocityToAngles(pRecord2->Velocity).y;
			const float flYaw3 = Math::VelocityToAngles(pRecord3->Velocity).y;
			const float flYaw4 = Math::VelocityToAngles(pRecord4->Velocity).y;

			const auto inc{ flYaw4 > flYaw3 && flYaw3 > flYaw2 && flYaw2 > flYaw1 && flYaw1 > flYaw0 };
			const auto dec{ flYaw4 < flYaw3 && flYaw3 < flYaw2 && flYaw2 < flYaw1 && flYaw1 < flYaw0 };

			if (!inc && !dec)
			{
				return;
			}

			const float flYawRate = (((flYaw0 - flYaw1) + (flYaw2 - flYaw3) + (flYaw3 - flYaw4)) / 3) / (TICK_INTERVAL * 50.0f);

			if (fabsf(flYawRate) < 1.0f)
			{
				return;
			}

			m_flYawTurnRate = std::clamp(flYawRate, -4.3f, 4.3f);
		}
	}

	if (CFG::Aimbot_Projectile_Air_Strafe_Prediction && !(m_PlayerDataBackup.m_fFlags & FL_ONGROUND) && F::LagRecords->HasRecords(pPlayer))
	{
		// Define number of records based on sv_airaccelerate value.
		const int numRecords = (I::CVar->FindVar("sv_airaccelerate")->GetFloat() != 10.0f) ? 15 : 5;

		// Use a dynamic array to store the records.
		std::vector<const LagRecord_t*> records(numRecords);

		// Retrieve the last numRecords lag records.
		for (int i = 0; i < numRecords; ++i)
		{
			records[i] = F::LagRecords->GetRecord(pPlayer, i);
		}

		// Ensure all records are valid.
		if (std::all_of(records.begin(), records.end(), [](const LagRecord_t* rec) { return rec != nullptr; }))
		{
			std::vector<float> yaws(numRecords);

			// Extract yaw values and normalize them.
			for (int i = 0; i < numRecords; ++i)
			{
				yaws[i] = Math::VelocityToAngles(records[i]->Velocity).y;
				if (yaws[i] < 0) yaws[i] += 360.0f;  // Normalize yaw to 0-360 range.
			}

			// Determine if the player is changing direction rapidly.
			bool rapidChange = false;
			for (int i = 1; i < numRecords; ++i)
			{
				if (std::abs(yaws[i] - yaws[i - 1]) > 90.0f) // If yaw change is too abrupt
				{
					rapidChange = true;
					break;
				}
			}

			// Calculate the yaw delta based on yaw behavior.
			float delta = 0.0f;
			if (rapidChange)
			{
				// Use last valid record for prediction if direction is rapidly changing.
				delta = yaws.back() - yaws.front(); // Simple linear extrapolation
			}
			else
			{
				// Average calculation for normal behavior.
				for (int i = 1; i < numRecords; ++i)
				{
					float diff = yaws[i - 1] - yaws[i];
					// Handle yaw wrapping.
					if (std::abs(diff) > 180.0f)
					{
						diff += (diff > 0.0f) ? -360.0f : 360.0f;
					}
					delta += diff;
				}
				delta /= (numRecords - 1);
			}

			m_flYawTurnRate = delta;

			// Adjust strafing based on the determined yaw behavior.
			m_MoveData.m_flSideMove = (m_flYawTurnRate > 0.0f) ? -450.0f : 450.0f; // Normal strafing.
			m_MoveData.m_flForwardMove = 0.0f; // Forward movement is disabled during this prediction.

			// Include movement speed factor for long-range prediction
			float speedFactor = 0.5f; // Adjust this factor for the desired sensitivity
			float adjustedYawTurnRate = m_flYawTurnRate * speedFactor;

			// Apply the adjusted yaw turn rate to the side movement for better accuracy at long range.
			m_MoveData.m_flSideMove *= adjustedYawTurnRate / 100.0f; // Scale the side movement for accuracy.
		}
	}





}

bool CMovementSimulation::Initialize(C_TFPlayer* pPlayer)
{
	if (!pPlayer || pPlayer->deadflag())
		return false;

	//set player
	m_pPlayer = pPlayer;

	//set current command
	//we'll use this to set current player's command, without it CGameMovement::CheckInterval will try to access a nullptr
	static CUserCmd dummyCmd = {};
	m_pPlayer->SetCurrentCommand(&dummyCmd);

	//store player's data
	m_PlayerDataBackup.Store(m_pPlayer);

	//store vars
	m_bOldInPrediction = I::Prediction->m_bInPrediction;
	m_bOldFirstTimePredicted = I::Prediction->m_bFirstTimePredicted;
	m_flOldFrametime = I::GlobalVars->frametime;

	//the hacks that make it work
	{
		if (pPlayer->m_fFlags() & FL_DUCKING)
		{
			pPlayer->m_fFlags() &= ~FL_DUCKING; //breaks origin's z if FL_DUCKING is not removed
			pPlayer->m_bDucked() = true; //(mins/maxs will be fine when ducking as long as m_bDucked is true)
			pPlayer->m_flDucktime() = 0.0f;
			pPlayer->m_flDuckJumpTime() = 0.0f;
			pPlayer->m_bDucking() = false;
			pPlayer->m_bInDuckJump() = true;
		}

		if (pPlayer != H::Entities->GetLocal())
			pPlayer->m_hGroundEntity() = nullptr; //without this nonlocal entities get snapped to the floor

		pPlayer->m_flModelScale() -= 0.03125f; //fixes issues with corners

		if (pPlayer->m_fFlags() & FL_ONGROUND)
			pPlayer->m_vecOrigin().z += 0.03125f * 3.0f; //to prevent getting stuck in the ground

		//for some reason if xy vel is zero it doesn't predict
		if (fabsf(pPlayer->m_vecVelocity().x) < 0.01f)
			pPlayer->m_vecVelocity().x = 0.015f;

		if (fabsf(pPlayer->m_vecVelocity().y) < 0.01f)
			pPlayer->m_vecVelocity().y = 0.015f;

		if ((pPlayer->m_fFlags() & FL_ONGROUND) || pPlayer->m_hGroundEntity().Get())
		{
			pPlayer->m_vecVelocity().z = 0.0f;
		}
	}

	//setup move data
	SetupMoveData(m_pPlayer, &m_MoveData);

	return true;
}

void CMovementSimulation::Restore()
{
	if (!m_pPlayer)
		return;

	m_pPlayer->SetCurrentCommand(nullptr);

	m_PlayerDataBackup.Restore(m_pPlayer);

	I::Prediction->m_bInPrediction = m_bOldInPrediction;
	I::Prediction->m_bFirstTimePredicted = m_bOldFirstTimePredicted;
	I::GlobalVars->frametime = m_flOldFrametime;

	m_pPlayer = nullptr;
	m_flYawTurnRate = 0.0f;

	std::memset(&m_MoveData, 0, sizeof(CMoveData));
	std::memset(&m_PlayerDataBackup, 0, sizeof(CPlayerDataBackup));
}



void CMovementSimulation::RunTick(float flTimeToTarget)
{
	if (!m_pPlayer)
	{
		return;
	}

	//make sure frametime and prediction vars are right
	I::Prediction->m_bInPrediction = true;
	I::Prediction->m_bFirstTimePredicted = false;
	I::GlobalVars->frametime = I::Prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL;

	if (m_MoveData.m_vecVelocity.Length() < 15.0f && (m_pPlayer->m_fFlags() & FL_ONGROUND))
	{
		return;
	}

	if (CFG::Aimbot_Projectile_Ground_Strafe_Prediction && (m_PlayerDataBackup.m_fFlags & FL_ONGROUND) && (m_pPlayer->m_fFlags() & FL_ONGROUND))
	{
		m_MoveData.m_vecViewAngles.y += m_flYawTurnRate * Math::RemapValClamped(flTimeToTarget, 0.0f, 1.0f, 1.0f, 0.5f);
	}

	if (CFG::Aimbot_Projectile_Air_Strafe_Prediction && !(m_PlayerDataBackup.m_fFlags & FL_ONGROUND) && !(m_pPlayer->m_fFlags() & FL_ONGROUND))
	{
		m_MoveData.m_vecViewAngles.y += m_flYawTurnRate;
	}

	m_bRunning = true;

	I::GameMovement->ProcessMovement(m_pPlayer, &m_MoveData);

	m_bRunning = false;
}


