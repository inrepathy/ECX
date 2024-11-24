#include "Misc.h"


#include <vector>
#include "../CFG.h"
#include "../../SEOwnedDE/src/SDK/SDK.h"


void CMisc::Bunnyhop(CUserCmd* pCmd)
{
	if (!CFG::Misc_Bunnyhop)
		return;

	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (pLocal->deadflag() || pLocal->m_nWaterLevel() > static_cast<byte>(WL_Feet))
			return;

		static bool bJumpState = false;

		if (pCmd->buttons & IN_JUMP)
		{
			if (!bJumpState && !(pLocal->m_fFlags() & FL_ONGROUND))
				pCmd->buttons &= ~IN_JUMP;

			else if (bJumpState)
				bJumpState = false;
		}

		else if (!bJumpState)
		{
			bJumpState = true;
		}
	}
}

void CMisc::FakeDuck(CUserCmd* pCmd)
{
	if (CFG::FakeDuck) {
		if (!H::Input->IsDown(CFG::FakeDuck_Key))
			return;

		bool* pSendPacket = reinterpret_cast<bool*>(uintptr_t(_AddressOfReturnAddress()) + 0x128);

		const auto pLocal = H::Entities->GetLocal();


		static auto active{ false };



		*pSendPacket = false;
		if (pLocal->m_bDucking() && pLocal->m_vecViewOffset().z == 45 && I::ClientState->chokedcommands > 12)
		{
			pCmd->buttons &= ~IN_DUCK;
			*pSendPacket = true;
		}
		else
		{
			pCmd->buttons |= IN_DUCK;
		}

	}


}


static Vec3 savedPeekPosition;
static bool bReturning = false;
static bool bKeyHeldDown = false;

void CMisc::AutoPeek(CUserCmd* pCmd)
{
	const auto pLocal = H::Entities->GetLocal();

	if (CFG::AutoPeek)
	{
		if (H::Input->IsDown(CFG::AutoPeek_Key))
		{
			if (!bKeyHeldDown)
			{
				bKeyHeldDown = true;
				const Vec3 localPos = pLocal->GetAbsOrigin();
				savedPeekPosition = localPos;
				bReturning = false;
			}
		}
		else
		{
			bKeyHeldDown = false;
		}

		if ((pCmd->buttons & IN_ATTACK) && !bReturning)
		{
			bReturning = true;
		}

		if (bKeyHeldDown)
		{
			Vec3 boxMins = Vec3(-2.f, -2.f, 0.f);
			Vec3 boxMaxs = Vec3(2.f, 2.f, 3.f);
			QAngle orientation = QAngle(0, 0, 0);
			Color_t faceColor = bReturning ? Color_t(0, 255, 0, 100) : Color_t(255, 0, 0, 100);
			Color_t edgeColor = bReturning ? Color_t(0, 255, 0, 255) : Color_t(255, 0, 0, 255);

			I::DebugOverlay->AddBoxOverlay2(savedPeekPosition, boxMins, boxMaxs, orientation, faceColor, edgeColor, 0.1f);

			if (bReturning)
			{
				const Vec3 localPos = pLocal->GetAbsOrigin();
				if (localPos.DistTo(savedPeekPosition) < 7.f)
				{
					bReturning = false;
					return;
				}

				float movementScale = 1.0f;
				SDKUtils::WalkTo(pCmd, localPos, savedPeekPosition, movementScale);
			}
		}
	}
	else
	{
		bReturning = false;
		savedPeekPosition = Vec3(0, 0, 0);
		bKeyHeldDown = false;
	}
}


void CMisc::DuckInAir(CUserCmd* pCmd)
{
	if (!CFG::DuckInAir)
		return;

	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (pLocal->deadflag() || (pLocal->m_fFlags() & FL_ONGROUND))
			return;

		if (pLocal->m_nWaterLevel() > static_cast<byte>(WL_Feet) || pLocal->GetMoveType() != MOVETYPE_WALK)
			return;

		// hacky
		if (!(pCmd->buttons & IN_DUCK))
		{
			pCmd->buttons |= IN_DUCK;
		}
	}
}




void CMisc::SequenceFreeze(CUserCmd* pCmd, bool* pSendPacket) {
	if (!CFG::SequenceFreeze)
		return;

	if (!H::Input->IsDown(CFG::SequenceFreezeKey) ||
		I::EngineVGui->IsGameUIVisible() ||
		I::MatSystemSurface->IsCursorVisible() ||
		SDKUtils::BInEndOfMatch())
		return;

	static int freezeTicks = 0;
	static const int maxFreezeTicks = 16;

	if (freezeTicks > 0) {
		pCmd->tick_count += freezeTicks;
		*pSendPacket = false;
		freezeTicks--;
	}
	else {
		pCmd->tick_count = pCmd->command_number;
		*pSendPacket = true;

		if (H::Input->IsDown(CFG::SequenceFreezeKey)) {
			freezeTicks = maxFreezeTicks;
		}
	}
}










void CMisc::LegJitter(CUserCmd* pCmd)
{
	if (!CFG::LegJitter)
		return;

	const auto pLocal = H::Entities->GetLocal();

	if (!pLocal->m_nSolidType()  || pLocal->deadflag()) { return; }
	static bool pos = true;
	const float scale = pLocal->m_bDucked() ? 14.f : 1.0f;

	if (G::bFiring)
		return;

	
	if (pCmd->forwardmove == 0.f && pCmd->sidemove == 0.f && pLocal->m_vecVelocity().Length2D() < 10.f)
	{
		pos ? pCmd->forwardmove = scale : pCmd->forwardmove = -scale;
		pos ? pCmd->sidemove = scale : pCmd->sidemove = -scale;
		pos = !pos;
	}
}

void CMisc::AutoEdgebug(CUserCmd* pCmd)
{
	if (!CFG::AutoEdgebug)
		return;

	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (pLocal->deadflag() || (pLocal->m_fFlags() & FL_ONGROUND))
			return;

		if (pLocal->m_nWaterLevel() > static_cast<byte>(WL_Feet) || pLocal->GetMoveType() != MOVETYPE_WALK)
			return;
		Vec3 vLocalPos = pLocal->GetAbsOrigin();
		Vector vForward, vRight, vUp;
		Math::AngleVectors(pCmd->viewangles, &vForward, &vRight, &vUp);

		const float forwardDistance = 30.0f;   
		const float downwardDistance = 50.0f;  
		const float ledgeDistance = 5.0f;      
		const float edgeHeight = 15.0f;       

		Ray_t rayDown;
		trace_t traceDown;
		rayDown.Init(vLocalPos, vLocalPos - vUp * downwardDistance); 
		I::EngineTrace->TraceRay(rayDown, MASK_PLAYERSOLID, nullptr, &traceDown);
		Ray_t rayForward;
		trace_t traceForward;
		rayForward.Init(vLocalPos, vLocalPos + vForward * forwardDistance); 
		I::EngineTrace->TraceRay(rayForward, MASK_PLAYERSOLID, nullptr, &traceForward);

		Ray_t rayDiagonal;
		trace_t traceDiagonal;
		rayDiagonal.Init(vLocalPos, vLocalPos + vForward * forwardDistance - vUp * edgeHeight); 
		I::EngineTrace->TraceRay(rayDiagonal, MASK_PLAYERSOLID, nullptr, &traceDiagonal);

		if (traceForward.DidHit() && !traceDown.DidHit()) 
		{
			if (!(pCmd->buttons & IN_DUCK)) 
			{
				pCmd->buttons |= IN_DUCK;  
			}
			pCmd->forwardmove = 0.0f;
		}
		else if (traceDiagonal.DidHit() && !traceDown.DidHit())
		{
			if (!(pCmd->buttons & IN_DUCK)) 
			{
				pCmd->buttons |= IN_DUCK;  
			}
		}
	}
}



void CMisc::AutoStrafer(CUserCmd* pCmd)
{
	//credits: KGB

	if (!CFG::Misc_Auto_Strafe)
		return;

	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (pLocal->deadflag() || (pLocal->m_fFlags() & FL_ONGROUND))
			return;

		if (pLocal->m_nWaterLevel() > static_cast<byte>(WL_Feet) || pLocal->GetMoveType() != MOVETYPE_WALK)
			return;

		if (!(pLocal->m_afButtonLast() & IN_JUMP) && (pCmd->buttons & IN_JUMP))
			return;

		if (pCmd->buttons & IN_MOVELEFT || pCmd->buttons & IN_MOVERIGHT || pCmd->buttons & IN_FORWARD || pCmd->buttons & IN_BACK)
		{
			const float flForwardMove = pCmd->forwardmove;
			const float flSideMove = pCmd->sidemove;

			Vec3 vForward = {}, vRight = {};
			Math::AngleVectors(pCmd->viewangles, &vForward, &vRight, nullptr);

			vForward.z = vRight.z = 0.0f;

			vForward.Normalize();
			vRight.Normalize();

			Vec3 vWishDir = {};
			Math::VectorAngles({(vForward.x * flForwardMove) + (vRight.x * flSideMove), (vForward.y * flForwardMove) + (vRight.y * flSideMove), 0.0f}, vWishDir);

			Vec3 vCurDir = {};
			Math::VectorAngles(pLocal->m_vecVelocity(), vCurDir);

			const float flDirDelta = Math::NormalizeAngle(vWishDir.y - vCurDir.y);
			const float flTurnScale = Math::RemapValClamped(CFG::Misc_Auto_Strafe_Turn_Scale, 0.0f, 1.0f, 0.9f, 1.0f);
			const float flRotation = DEG2RAD((flDirDelta > 0.0f ? -90.0f : 90.f) + (flDirDelta * flTurnScale));

			const float flCosRot = cosf(flRotation);
			const float flSinRot = sinf(flRotation);

			pCmd->forwardmove = (flCosRot * flForwardMove) - (flSinRot * flSideMove);
			pCmd->sidemove = (flSinRot * flForwardMove) + (flCosRot * flSideMove);
		}
	}
}


// fuck you
void CMisc::CircleStrafer(CUserCmd* pCmd)
{

	if (!H::Input->IsDown(CFG::CircleStraferKey)
		|| I::EngineVGui->IsGameUIVisible()
		|| I::MatSystemSurface->IsCursorVisible()
		|| SDKUtils::BInEndOfMatch())
	{
		return;
	}
	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (pLocal->deadflag() || (pLocal->m_fFlags() & FL_ONGROUND))
			return;

		if (pLocal->m_nWaterLevel() > static_cast<byte>(WL_Feet) || pLocal->GetMoveType() != MOVETYPE_WALK)
			return;

		if (!(pLocal->m_afButtonLast() & IN_JUMP) && (pCmd->buttons & IN_JUMP))
			return;

		static float flCircleTime = 0.0f;
		flCircleTime += 0.07f; 
		if (flCircleTime > 360.0f) flCircleTime = 0.0f;

		const float flCos = cosf(DEG2RAD(flCircleTime));
		const float flSin = sinf(DEG2RAD(flCircleTime));

		const float flForwardMove = pCmd->forwardmove;
		const float flSideMove = pCmd->sidemove;

		pCmd->forwardmove = (flCos * flForwardMove) - (flSin * flSideMove);
		pCmd->sidemove = (flSin * flForwardMove) + (flCos * flSideMove);
	}
}



void CMisc::NoiseMakerSpam()
{
	if (!CFG::Misc_NoiseMaker_Spam)
		return;

	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (pLocal->deadflag() || pLocal->m_flNextNoiseMakerTime() >= I::GlobalVars->curtime)
			return;

		const auto kv = new KeyValues("use_action_slot_item_server");
		I::EngineClient->ServerCmdKeyValues(kv);
	}
}

void CMisc::FastStop(CUserCmd* pCmd)
{
	if (!CFG::Misc_Fast_Stop)
		return;

	if (const auto pLocal = H::Entities->GetLocal())
	{
		if (!pLocal->deadflag()
			&& pLocal->GetMoveType() == MOVETYPE_WALK
			&& (pLocal->m_fFlags() & FL_ONGROUND)
			&& pLocal->m_vecVelocity().Length2D() >= 10.0f
			&& !(pCmd->buttons & (IN_FORWARD | IN_BACK | IN_MOVERIGHT | IN_MOVELEFT)))
		{
			const Vector velocity = pLocal->m_vecVelocity();
			QAngle direction;

			Math::VectorAngles(velocity, direction);

			const float speed = velocity.Length();

			direction.y = I::EngineClient->GetViewAngles().y - direction.y;

			Vec3 forward{};

			Math::AngleVectors(direction, &forward);

			const Vector negatedDirection = forward * -speed;

			pCmd->forwardmove = negatedDirection.x;
			pCmd->sidemove = negatedDirection.y;
		}
	}
}

void CMisc::AutoRocketJump(CUserCmd* cmd)
{
	if (!H::Input->IsDown(CFG::Misc_Auto_Rocket_Jump_Key)
		|| I::EngineVGui->IsGameUIVisible()
		|| I::MatSystemSurface->IsCursorVisible()
		|| SDKUtils::BInEndOfMatch())
	{
		return;
	}

	const auto local{H::Entities->GetLocal()};

	if (!local
		|| local->deadflag()
		|| local->GetMoveType() != MOVETYPE_WALK
		|| local->m_nWaterLevel() > WL_Feet
		|| local->m_fFlags() & FL_DUCKING
		|| !(local->m_fFlags() & FL_ONGROUND)
		|| local->InCond(TF_COND_TAUNTING)
		|| local->InCond(TF_COND_HALLOWEEN_GHOST_MODE)
		|| local->InCond(TF_COND_HALLOWEEN_BOMB_HEAD)
		|| local->InCond(TF_COND_HALLOWEEN_KART))
	{
		return;
	}

	const auto weapon{H::Entities->GetWeapon()};

	if (!weapon
		|| (weapon->GetWeaponID() != TF_WEAPON_ROCKETLAUNCHER && weapon->GetWeaponID() != TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT)
		|| weapon->m_iItemDefinitionIndex() == Soldier_m_TheBeggarsBazooka
		|| !weapon->HasPrimaryAmmoForShot())
	{
		return;
	}

	if (weapon->m_iReloadMode() != 0)
	{
		cmd->buttons |= IN_ATTACK;
	}

	if (local->m_vecViewOffset().z > 45.65f)
	{
		cmd->buttons |= IN_DUCK;
	}

	else
	{
		auto pitch{Math::RemapValClamped(I::EngineClient->GetViewAngles().x, -89.0f, 0.0f, 89.0f, 50.0f)};
		auto yaw{Math::NormalizeAngle(Math::VelocityToAngles(local->m_vecVelocity()).y + 180.0f)};

		if (!(cmd->buttons & (IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT)))
		{
			yaw = Math::NormalizeAngle(I::EngineClient->GetViewAngles().y + 180.0f);
		}

		cmd->viewangles.Set(pitch, yaw, weapon->m_iItemDefinitionIndex() != Soldier_m_TheOriginal ? 90.0f : 0.0f);

		cmd->buttons |= IN_ATTACK | IN_DUCK | IN_JUMP;
		cmd->buttons &= ~IN_ATTACK2;

		G::bSilentAngles = true;
	}
}

void CMisc::AutoDisguise(CUserCmd* cmd)
{
	if (!CFG::Misc_Auto_Disguise || I::GlobalVars->tickcount % 20 != 0)
	{
		return;
	}

	const auto local{H::Entities->GetLocal()};

	if (!local || local->deadflag() || local->m_iClass() != TF_CLASS_SPY || local->InCond(TF_COND_DISGUISED) || local->InCond(TF_COND_DISGUISING))
	{
		return;
	}

	I::EngineClient->ClientCmd_Unrestricted("lastdisguise");
}

void CMisc::AutoMedigun(CUserCmd* cmd)
{
	static std::vector<C_TFPlayer*> potential{};

	if (!H::Input->IsDown(CFG::Misc_Auto_Medigun_Key))
	{
		if (!potential.empty())
		{
			potential.clear();
		}

		return;
	}

	if (I::EngineVGui->IsGameUIVisible() || I::MatSystemSurface->IsCursorVisible() || SDKUtils::BInEndOfMatch())
	{
		potential.clear();

		return;
	}

	const auto local{H::Entities->GetLocal()};

	if (!local
		|| local->deadflag()
		|| local->InCond(TF_COND_TAUNTING)
		|| local->InCond(TF_COND_HALLOWEEN_GHOST_MODE)
		|| local->InCond(TF_COND_HALLOWEEN_BOMB_HEAD)
		|| local->InCond(TF_COND_HALLOWEEN_KART))
	{
		potential.clear();

		return;
	}

	const auto weapon{H::Entities->GetWeapon()};

	if (!weapon || weapon->GetWeaponID() != TF_WEAPON_MEDIGUN)
	{
		potential.clear();

		return;
	}

	const auto medigun{weapon->As<C_WeaponMedigun>()};

	if (!medigun)
	{
		potential.clear();

		return;
	}

	auto isPlayerGood = [&](C_TFPlayer* pl)
	{
		if (!pl || pl->deadflag() || pl->GetCenter().DistTo(local->GetShootPos()) > 449.0f || pl->InCond(TF_COND_STEALTHED))
		{
			return false;
		}

		if (medigun->m_hHealingTarget().Get())
		{
			static C_BaseEntity* last{};

			if (medigun->m_bChargeRelease())
			{
				if (medigun->m_hHealingTarget().Get() == pl)
				{
					return false;
				}
			}

			else
			{
				if (pl->IsInvulnerable())
				{
					return false;
				}

				if (medigun->m_hHealingTarget().Get() == pl && last == pl)
				{
					return false;
				}

				auto mult{1.44f};

				if (medigun->GetMedigunType() == MEDIGUN_QUICKFIX)
				{
					mult = 1.24f;
				}

				if (static_cast<float>(pl->m_iHealth()) >= static_cast<float>(pl->GetMaxHealth()) * mult)
				{
					return false;
				}
			}

			last = medigun->m_hHealingTarget().Get();
		}

		CTraceFilterHitscan filter{};
		trace_t tr{};

		H::AimUtils->Trace(local->GetShootPos(), pl->GetCenter(), (MASK_SHOT & ~CONTENTS_HITBOX), &filter, &tr);

		return tr.fraction > 0.99f || tr.m_pEnt == pl;
	};

	if (potential.empty())
	{
		for (const auto ent : H::Entities->GetGroup(EEntGroup::PLAYERS_TEAMMATES))
		{
			if (!ent || ent == local)
			{
				continue;
			}

			auto pl{ent->As<C_TFPlayer>()};

			if (!isPlayerGood(pl))
			{
				continue;
			}

			potential.push_back(pl);
		}
	}

	if ((cmd->tick_count % 2) && G::bCanPrimaryAttack && !potential.empty())
	{
		for (int n{0}; n < static_cast<int>(potential.size()); n++)
		{
			const auto pl{potential[n]};

			if (!isPlayerGood(pl))
			{
				potential.erase(potential.begin() + n);

				continue;
			}

			const auto angle{Math::CalcAngle(local->GetShootPos(), pl->GetCenter())};

			H::AimUtils->FixMovement(cmd, angle);

			cmd->viewangles = angle;
			cmd->buttons |= IN_ATTACK;

			G::bPSilentAngles = true;

			if (CFG::Misc_Accuracy_Improvements)
			{
				cmd->tick_count = TIME_TO_TICKS(pl->m_flSimulationTime() + SDKUtils::GetLerp());
			}

			break;
		}
	}

	else
	{
		cmd->buttons &= ~IN_ATTACK;
	}
}

void CMisc::MovementLock(CUserCmd* cmd)
{
	static auto active{false};

	if (!CFG::Misc_Movement_Lock_Key)
	{
		active = false;

		return;
	}

	const auto local{H::Entities->GetLocal()};

	if (!local || (local->m_fFlags() & FL_ONGROUND))
	{
		active = false;

		return;
	}

	const auto& vel{local->m_vecVelocity()};
	static Vec3 lastTickVel{};

	static auto angles{cmd->viewangles};

	if (!active
		&& static_cast<int>(vel.x) == 0 && static_cast<int>(vel.y) == 0 && static_cast<int>(vel.z) == -6
		&& static_cast<int>(lastTickVel.x) == 0 && static_cast<int>(lastTickVel.y) == 0 && static_cast<int>(lastTickVel.z) == -6)
	{
		active = true;

		angles = cmd->viewangles;

		if (cmd->buttons & IN_MOVELEFT)
		{
			angles.y += 45.0f;
		}

		if (cmd->buttons & IN_MOVERIGHT)
		{
			angles.y -= 45.0f;
		}
	}

	lastTickVel = vel;

	if (active && static_cast<int>(vel.x) != 0 && static_cast<int>(vel.y) != 0 && static_cast<int>(vel.z) != -6)
	{
		active = false;
	}

	if (H::Input->IsPressed(CFG::Misc_Movement_Lock_Key) && !I::MatSystemSurface->IsCursorVisible() && !I::EngineVGui->IsGameUIVisible())
	{
		active = false;
	}

	if (!active)
	{
		return;
	}

	const auto angle = DEG2RAD((angles.y - cmd->viewangles.y) + 90.0f);

	cmd->forwardmove = sinf(angle) * 450.0f;
	cmd->sidemove = cosf(angle) * 450.0f;
}

void CMisc::MvmInstaRespawn()
{
	if (!H::Input->IsDown(CFG::Misc_MVM_Instant_Respawn_Key)
		|| I::EngineVGui->IsGameUIVisible()
		|| I::MatSystemSurface->IsCursorVisible()
		|| SDKUtils::BInEndOfMatch())
	{
		return;
	}

	auto* kv{new KeyValues("MVM_Revive_Response")};

	kv->SetInt("accepted", 1);

	I::EngineClient->ServerCmdKeyValues(kv);
}


void CMisc::FastAccel(CUserCmd* pCmd)
{
	if (CFG::FastAccel) {
		const auto pLocal = H::Entities->GetLocal();
		const auto pWeapon = H::Entities->GetWeapon();

		bFastAccel = false;

		// check valid team, swim stuff and bCanPrimaryAttack
		if (!pLocal->IsInValidTeam() || pLocal->m_flSwimSoundTime() || G::bCanPrimaryAttack) 
		{
			return;
		}

		if (pLocal->m_flItemChargeMeter())
		{   //  demoman charge
			return;
		}

		if (pLocal->GetMoveType() == MOVETYPE_NOCLIP
			|| pLocal->GetMoveType() == MOVETYPE_LADDER
			|| pLocal->GetMoveType() == MOVETYPE_OBSERVER)
		{
			return;
		}

		const int maxSpeed = std::min(pLocal->m_flMaxspeed() * (pCmd->forwardmove < 0 ? .9f : 1.f) - 1, 510.f); // get max speed
		const float curSpeed = pLocal->m_vecVelocity().Length2D();

		if (curSpeed > maxSpeed)
		{
			return; // no need to accelerate if at max speed
		}

		if (pCmd->buttons & (IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK))
		{
			const Vec3 vecMove(pCmd->forwardmove, pCmd->sidemove, 0.0f);
			const float flLength = vecMove.Length();
			Vec3 angMoveReverse;
			Math::VectorAngles(vecMove * -1.f, angMoveReverse);
			pCmd->forwardmove = -flLength;
			pCmd->sidemove = 0.0f;
			pCmd->viewangles.y = fmodf(pCmd->viewangles.y - angMoveReverse.y, 360.0f); // this is clamped automatically by the engine sooooo
			pCmd->viewangles.z = 270.f;
			G::bUpdatingAnims = false;
			bFastAccel = true;
		}
	}
}

void CMisc::RageRetry()
{
	const auto pLocal = H::Entities->GetLocal();

	if (!pLocal) 
		return;

	if (pLocal->deadflag()) 
		return;

	if (CFG::RageRetry)
	{
		if (pLocal->m_iHealth() <= (pLocal->GetMaxHealth() * (CFG::RageRetryHealth * 0.01f)))
		{
			I::EngineClient->ClientCmd_Unrestricted("retry");
		}
	}
}



void CMisc::AntiAfk(CUserCmd* pCmd)
{
	if (CFG::AntiAfk && I::CVar->FindVar("mp_idlemaxtime") != 0)
	{
		if (pCmd->command_number % 2)
		{
			pCmd->buttons |= 1 << 27;
		}
	}
}
