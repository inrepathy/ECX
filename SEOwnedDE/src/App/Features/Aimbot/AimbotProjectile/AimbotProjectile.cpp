#include "AimbotProjectile.h"

#include "../../CFG.h"
#include "../../MovementSimulation/MovementSimulation.h"
#include "../../ProjectileSim/ProjectileSim.h"

#include <cstdlib>  // rand()

void DrawProjPath(const CUserCmd* pCmd, float time)
{
	if (!pCmd || !G::bFiring)
	{
		return;
	}

	const auto pLocal = H::Entities->GetLocal();
	if (!pLocal || pLocal->deadflag())
	{
		return;
	}

	const auto pWeapon = H::Entities->GetWeapon();
	if (!pWeapon)
	{
		return;
	}

	ProjectileInfo info = {};
	if (!F::ProjectileSim->GetInfo(pLocal, pWeapon, pCmd->viewangles, info))
	{
		return;
	}

	if (!F::ProjectileSim->Init(info))
	{
		return;
	}

	for (auto n = 0; n < TIME_TO_TICKS(time); n++)
	{
		auto pre{ F::ProjectileSim->GetOrigin() };

		F::ProjectileSim->RunTick();

		auto post{ F::ProjectileSim->GetOrigin() };

		I::DebugOverlay->AddLineOverlay(pre, post, 255, 255, 255, false, 10.0f);
	}
}



void DrawMovePath(const std::vector<Vec3>& vPath)
{
	const Color_t color = CFG::MovementPath_Color;

	if (vPath.size() < 2)
		return;

	Color_t startColor = color; 
	Color_t endColor = color; 

	if (CFG::LongColor_Enabled)
	{
		endColor = (CFG::LongPath_Color.r != 0 || CFG::LongPath_Color.g != 0 || CFG::LongPath_Color.b != 0) ? CFG::LongPath_Color : color;
	}

	auto InterpolateColor = [](const Color_t& start, const Color_t& end, float t) -> Color_t
		{
			return Color_t{
				static_cast<unsigned char>(start.r + (end.r - start.r) * t),
				static_cast<unsigned char>(start.g + (end.g - start.g) * t),
				static_cast<unsigned char>(start.b + (end.b - start.b) * t),
				static_cast<unsigned char>(start.a + (end.a - start.a) * t)
			};
		};

	// Line style
	if (CFG::Visuals_Draw_Movement_Path_Style == 1)
	{
		for (size_t n = 1; n < vPath.size(); n++)
		{
			float t = static_cast<float>(n) / static_cast<float>(vPath.size() - 1); // Calculate the interpolation factor
			Color_t currentColor = CFG::LongColor_Enabled ? InterpolateColor(startColor, endColor, t) : startColor; // Apply gradient if enabled

			I::DebugOverlay->AddLineOverlay(vPath[n], vPath[n - 1], currentColor.r, currentColor.g, currentColor.b, false, 10.0f);
		}
	}

	// Dashed line style
	if (CFG::Visuals_Draw_Movement_Path_Style == 2)
	{
		for (size_t n = 1; n < vPath.size(); n++)
		{
			if (n % 2 == 0)
			{
				continue;
			}

			float t = static_cast<float>(n) / static_cast<float>(vPath.size() - 1);
			Color_t currentColor = CFG::LongColor_Enabled ? InterpolateColor(startColor, endColor, t) : startColor;

			I::DebugOverlay->AddLineOverlay(vPath[n], vPath[n - 1], currentColor.r, currentColor.g, currentColor.b, false, 10.0f);
		}
	}

	// Alternative line style
	if (CFG::Visuals_Draw_Movement_Path_Style == 3)
	{
		for (size_t n = 1; n < vPath.size(); n++)
		{
			float t = static_cast<float>(n) / static_cast<float>(vPath.size() - 1);
			Color_t currentColor = CFG::LongColor_Enabled ? InterpolateColor(startColor, endColor, t) : startColor;

			if (n != 1)
			{
				Vec3 right{};
				Math::AngleVectors(Math::CalcAngle(vPath[n], vPath[n - 1]), nullptr, &right, nullptr);

				const Vec3& start{ vPath[n - 1] };
				const Vec3 endL{ vPath[n - 1] + (right * 5.0f) };
				const Vec3 endR{ vPath[n - 1] - (right * 5.0f) };

				I::DebugOverlay->AddLineOverlay(start, endL, currentColor.r, currentColor.g, currentColor.b, false, 10.0f);
				I::DebugOverlay->AddLineOverlay(start, endR, currentColor.r, currentColor.g, currentColor.b, false, 10.0f);
			}

			I::DebugOverlay->AddLineOverlay(vPath[n], vPath[n - 1], currentColor.r, currentColor.g, currentColor.b, false, 10.0f);
		}
	}
}




Vec3 GetOffsetShootPos(C_TFPlayer* local, C_TFWeaponBase* weapon, const CUserCmd* pCmd)
{
	auto out{ local->GetShootPos() };

	switch (weapon->GetWeaponID())
	{
		case TF_WEAPON_ROCKETLAUNCHER:
		case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
		case TF_WEAPON_FLAREGUN:
		case TF_WEAPON_FLAREGUN_REVENGE:
		case TF_WEAPON_SYRINGEGUN_MEDIC:
		case TF_WEAPON_FLAME_BALL:
		case TF_WEAPON_CROSSBOW:
		case TF_WEAPON_FLAMETHROWER:
		case TF_WEAPON_SHOTGUN_BUILDING_RESCUE:
		{
			if (weapon->m_iItemDefinitionIndex() != Soldier_m_TheOriginal)
			{
				Vec3 vOffset = { 23.5f, 12.0f, -3.0f };

				if (local->m_fFlags() & FL_DUCKING)
					vOffset.z = 8.0f;

				H::AimUtils->GetProjectileFireSetup(pCmd->viewangles, vOffset, &out);
			}

			break;
		}

		case TF_WEAPON_COMPOUND_BOW:
		{
			Vec3 vOffset = { 20.5f, 12.0f, -3.0f };

			if (local->m_fFlags() & FL_DUCKING)
				vOffset.z = 8.0f;

			H::AimUtils->GetProjectileFireSetup(pCmd->viewangles, vOffset, &out);

			break;
		}

		default: break;
	}

	return out;
}

bool CAimbotProjectile::GetProjectileInfo(C_TFWeaponBase* pWeapon)
{
	m_CurProjInfo = {};

	auto curTime = [&]() -> float
	{
		if (const auto pLocal = H::Entities->GetLocal())
		{
			return static_cast<float>(pLocal->m_nTickBase()) * I::GlobalVars->interval_per_tick;
		}

		return I::GlobalVars->curtime;
	};

	switch (pWeapon->GetWeaponID())
	{
		case TF_WEAPON_ROCKETLAUNCHER:
		case TF_WEAPON_PARTICLE_CANNON:
		case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
		{
			m_CurProjInfo = { 1100.0f, 0.0f };
			m_CurProjInfo.Speed = SDKUtils::AttribHookValue(m_CurProjInfo.Speed, "mult_projectile_speed", pWeapon);

			if (C_TFPlayer* local{ H::Entities->GetLocal() })
			{
				if (const int rocket_specialist{ static_cast<int>(SDKUtils::AttribHookValue(0.0f, "rocket_specialist", local)) })
				{
					m_CurProjInfo.Speed *= Math::RemapValClamped(static_cast<float>(rocket_specialist), 1.0f, 4.0f, 1.15f, 1.6f);
					m_CurProjInfo.Speed = std::min(m_CurProjInfo.Speed, 3000.0f);
				}
			}

			break;
		}

		case TF_WEAPON_GRENADELAUNCHER:
		{
			m_CurProjInfo = { 1200.0f, 1.0f, true };
			m_CurProjInfo.Speed = SDKUtils::AttribHookValue(m_CurProjInfo.Speed, "mult_projectile_speed", pWeapon);

			break;
		}

		case TF_WEAPON_PIPEBOMBLAUNCHER:
		{
			const float flChargeBeginTime = pWeapon->As<C_TFPipebombLauncher>()->m_flChargeBeginTime();
			const float flCharge = curTime() - flChargeBeginTime;

			if (flChargeBeginTime)
			{
				m_CurProjInfo.Speed = Math::RemapValClamped
				(
					flCharge,
					0.0f,
					SDKUtils::AttribHookValue(4.0f, "stickybomb_charge_rate", pWeapon),
					900.0f,
					2400.0f
				);
			}

			else
			{
				m_CurProjInfo.Speed = 900.0f;
			}

			m_CurProjInfo.GravityMod = 1.0f;
			m_CurProjInfo.Pipes = true;

			break;
		}

		case TF_WEAPON_CANNON:
		{
			m_CurProjInfo = { 1454.0f, 1.0f, true };
			break;
		}

		case TF_WEAPON_COMPOUND_BOW:
		{
			const float flChargeBeginTime = pWeapon->As<C_TFPipebombLauncher>()->m_flChargeBeginTime();
			const float flCharge = curTime() - flChargeBeginTime;

			if (flChargeBeginTime)
			{
				m_CurProjInfo.Speed = 1800.0f + std::clamp<float>(flCharge, 0.0f, 1.0f) * 800.0f;
				m_CurProjInfo.GravityMod = Math::RemapValClamped(flCharge, 0.0f, 1.0f, 0.5f, 0.1f);
			}

			else
			{
				m_CurProjInfo.Speed = 1800.0f;
				m_CurProjInfo.GravityMod = 0.5f;
			}

			break;
		}

		case TF_WEAPON_CROSSBOW:
		case TF_WEAPON_SHOTGUN_BUILDING_RESCUE:
		{
			m_CurProjInfo = { 2400.0f, 0.2f };
			break;
		}

		case TF_WEAPON_SYRINGEGUN_MEDIC:
		{
			m_CurProjInfo = { 1000.0f, 0.3f };
			break;
		}

		case TF_WEAPON_FLAREGUN:
		{
			m_CurProjInfo = { 2000.0f, 0.3f };
			break;
		}

		case TF_WEAPON_FLAREGUN_REVENGE:
		{
			m_CurProjInfo = { 3000.0f, 0.45f };
			break;
		}

		case TF_WEAPON_FLAME_BALL:
		{
			m_CurProjInfo = { 3000.0f, 0.0f };
			break;
		}

		case TF_WEAPON_FLAMETHROWER:
		{
			m_CurProjInfo = { 2000.0f, 0.0f };
			m_CurProjInfo.Flamethrower = true;
			break;
		}

		case TF_WEAPON_RAYGUN:
		case TF_WEAPON_DRG_POMSON:
		{
			m_CurProjInfo = { 1200.0f, 0.0f };

			break;
		}

		default: break;
	}

	return m_CurProjInfo.Speed > 0.0f;
}

bool CAimbotProjectile::CalcProjAngle(const Vec3& vFrom, const Vec3& vTo, Vec3& vAngleOut, float& flTimeOut)
{
	const auto pWeapon = H::Entities->GetWeapon();
	if (!pWeapon)
		return false;

	const Vec3 v = vTo - vFrom;
	const float dx = sqrt(v.x * v.x + v.y * v.y);
	const float dy = v.z;
	float v0 = m_CurProjInfo.Speed;
	const float g = SDKUtils::GetGravity() * m_CurProjInfo.GravityMod;

	if (g)
	{
		if (m_CurProjInfo.Pipes)
		{
			if (v0 > k_flMaxVelocity)
			{
				v0 = k_flMaxVelocity;
			}
		}

		const auto root{ v0 * v0 * v0 * v0 - g * (g * dx * dx + 2.0f * dy * v0 * v0) };
		if (root < 0.0f)
		{
			return false;
		}

		vAngleOut = { -RAD2DEG(atanf((v0 * v0 - sqrtf(root)) / (g * dx))), RAD2DEG(atan2f(v.y, v.x)), 0.0f };
		flTimeOut = dx / (cosf(-DEG2RAD(vAngleOut.x)) * v0);

		if (m_CurProjInfo.Pipes)
		{
			//do 2nd pass for drag | TODO: Math > Magic

			auto magic{ 0.0f };

			if (pWeapon->GetWeaponID() == TF_WEAPON_GRENADELAUNCHER)
			{
				if (pWeapon->m_iItemDefinitionIndex() == Demoman_m_TheLochnLoad)
				{
					magic = 0.07f;
				}

				else
				{
					magic = 0.11f;
				}
			}

			if (pWeapon->GetWeaponID() == TF_WEAPON_PIPEBOMBLAUNCHER)
			{
				magic = 0.16f;
			}

			if (pWeapon->GetWeaponID() == TF_WEAPON_CANNON)
			{
				magic = 0.35f;
			}

			v0 -= (v0 * flTimeOut) * magic;

			auto root{ v0 * v0 * v0 * v0 - g * (g * dx * dx + 2.0f * dy * v0 * v0) };

			if (root < 0.0f)
			{
				return false;
			}

			vAngleOut = { -RAD2DEG(atanf((v0 * v0 - sqrtf(root)) / (g * dx))), RAD2DEG(atan2f(v.y, v.x)), 0.0f };
			flTimeOut = dx / (cosf(-DEG2RAD(vAngleOut.x)) * v0);
		}
	}

	else
	{
		vAngleOut = Math::CalcAngle(vFrom, vTo);
		flTimeOut = vFrom.DistTo(vTo) / v0;
	}

	if (m_CurProjInfo.Pipes)
	{
		if (pWeapon->GetWeaponID() == TF_WEAPON_CANNON)
		{
			if (flTimeOut > 0.95f)
			{
				return false;
			}
		}
		else
		{
			if (pWeapon->m_iItemDefinitionIndex() == Demoman_m_TheIronBomber)
			{
				if (flTimeOut > 1.4f)
				{
					return false;
				}
			}

			else
			{
				if (flTimeOut > 2.0f)
				{
					return false;
				}
			}
		}
	}

	if ((pWeapon->GetWeaponID() == TF_WEAPON_FLAME_BALL || pWeapon->GetWeaponID() == TF_WEAPON_FLAMETHROWER) && flTimeOut > 0.18f)
	{
		return false;
	}

	return true;
}


void CAimbotProjectile::OffsetPlayerPosition(C_TFWeaponBase* pWeapon, Vec3& vPos, C_TFPlayer* pPlayer, bool bDucked, bool bOnGround)
{
	const float flMaxZ{ (bDucked ? 62.0f : 82.0f) * pPlayer->m_flModelScale() };

	switch (CFG::Aimbot_Projectile_Aim_Position)
	{
		// Feet
		case 0:
		{
			vPos.z += (flMaxZ * 0.2f);
			m_LastAimPos = 0;
			break;
		}

		// Body
		case 1:
		{
			vPos.z += (flMaxZ * 0.5f);
			m_LastAimPos = 1;
			break;
		}

		// Head
		case 2:
		{
			if (CFG::Aimbot_Projectile_Advanced_Head_Aim)
			{
				const Vec3 vDelta = pPlayer->GetHitboxPos(HITBOX_HEAD) - pPlayer->m_vecOrigin();
				vPos.x += vDelta.x;
				vPos.y += vDelta.y;
			}

			vPos.z += (flMaxZ * 0.85f);
			m_LastAimPos = 2;
			break;
		}

		// Auto
		case 3:
		{
			if (pWeapon->GetWeaponID() == TF_WEAPON_COMPOUND_BOW)
			{
				if (CFG::Aimbot_Projectile_Advanced_Head_Aim)
				{
					const Vec3 vDelta = pPlayer->GetHitboxPos(HITBOX_HEAD) - pPlayer->m_vecOrigin();
					vPos.x += vDelta.x;
					vPos.y += vDelta.y;
				}

				vPos.z += (flMaxZ * 0.92f);
				m_LastAimPos = 2;
			}

			else
			{
				switch (pWeapon->GetWeaponID())
				{
					case TF_WEAPON_ROCKETLAUNCHER:
					case TF_WEAPON_PARTICLE_CANNON:
					case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
					case TF_WEAPON_GRENADELAUNCHER:
					case TF_WEAPON_CANNON:
					{
						if (bOnGround)
						{
							vPos.z += (flMaxZ * 0.2f);
							m_LastAimPos = 0;
						}

						else
						{
							vPos.z += (flMaxZ * 0.5f);
							m_LastAimPos = 1;
						}

						break;
					}
					case TF_WEAPON_PIPEBOMBLAUNCHER:
					{
						vPos.z += (flMaxZ * 0.1f);
						m_LastAimPos = 0;
						break;
					}

					default:
					{
						vPos.z += (flMaxZ * 0.5f);
						m_LastAimPos = 1;
						break;
					}
				}
			}

			break;
		}

		default: break;
	}
}

bool CAimbotProjectile::CanArcReach(const Vec3& vFrom, const Vec3& vTo, const Vec3& vAngleTo, float flTargetTime, C_BaseEntity* pTarget)
{
	const auto pLocal = H::Entities->GetLocal();
	if (!pLocal)
	{
		return false;
	}

	const auto pWeapon = H::Entities->GetWeapon();
	if (!pWeapon)
	{
		return false;
	}

	ProjectileInfo info{};
	if (!F::ProjectileSim->GetInfo(pLocal, pWeapon, vAngleTo, info))
	{
		return false;
	}

	if (pWeapon->m_iItemDefinitionIndex() == Demoman_m_TheLochnLoad)
	{
		info.m_speed += 45.0f; //need to do this for some reason
	}

	if (!F::ProjectileSim->Init(info, true))
	{
		return false;
	}

	CTraceFilterWorldCustom filter{};
	filter.m_pTarget = pTarget;

	//I::DebugOverlay->ClearAllOverlays();

	for (auto n = 0; n < TIME_TO_TICKS(flTargetTime * 1.2f); n++)
	{
		auto pre{ F::ProjectileSim->GetOrigin() };

		F::ProjectileSim->RunTick();

		auto post{ F::ProjectileSim->GetOrigin() };

		trace_t trace{};

		Vec3 mins{ -6.0f, -6.0f, -6.0f };
		Vec3 maxs{ 6.0f, 6.0f, 6.0f };

		switch (info.m_type)
		{
			case TF_PROJECTILE_PIPEBOMB:
			case TF_PROJECTILE_PIPEBOMB_REMOTE:
			case TF_PROJECTILE_PIPEBOMB_PRACTICE:
			case TF_PROJECTILE_CANNONBALL:
			{
				mins = { -8.0f, -8.0f, -8.0f };
				maxs = { 8.0f, 8.0f, 20.0f };

				break;
			}

			case TF_PROJECTILE_FLARE:
			{
				mins = { -8.0f, -8.0f, -8.0f };
				maxs = { 8.0f, 8.0f, 8.0f };

				break;
			}

			default:
			{
				break;
			}
		}

		H::AimUtils->TraceHull(pre, post, mins, maxs, MASK_SOLID, &filter, &trace);

		if (trace.m_pEnt == pTarget)
		{
			return true;
		}

		if (trace.DidHit())
		{
			if (info.m_pos.DistTo(trace.endpos) > info.m_pos.DistTo(vTo))
			{
				return true;
			}

			if (trace.endpos.DistTo(vTo) > 40.0f)
			{
				return false;
			}

			H::AimUtils->Trace(trace.endpos, vTo, MASK_SOLID, &filter, &trace);

			return !trace.DidHit() || trace.m_pEnt == pTarget;
		}

		//I::DebugOverlay->AddBoxOverlay(post, mins, maxs, Math::CalcAngle(pre, post), 255, 255, 255, 2, 60.0f);
	}

	return true;
}

bool CAimbotProjectile::CanSee(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, const Vec3& vFrom, const Vec3& vTo, const ProjTarget_t& target, float flTargetTime)
{
	Vec3 vLocalPos = vFrom;

	switch (pWeapon->GetWeaponID())
	{
		case TF_WEAPON_ROCKETLAUNCHER:
		case TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT:
		case TF_WEAPON_FLAREGUN:
		case TF_WEAPON_FLAREGUN_REVENGE:
		case TF_WEAPON_SYRINGEGUN_MEDIC:
		case TF_WEAPON_FLAME_BALL:
		case TF_WEAPON_CROSSBOW:
		case TF_WEAPON_FLAMETHROWER:
		case TF_WEAPON_SHOTGUN_BUILDING_RESCUE:
		{
			if (pWeapon->m_iItemDefinitionIndex() != Soldier_m_TheOriginal)
			{
				Vec3 vOffset = { 23.5f, 12.0f, -3.0f };

				if (pLocal->m_fFlags() & FL_DUCKING)
					vOffset.z = 8.0f;

				H::AimUtils->GetProjectileFireSetup(target.AngleTo, vOffset, &vLocalPos);
			}

			break;
		}

		case TF_WEAPON_COMPOUND_BOW:
		{
			Vec3 vOffset = { 20.5f, 12.0f, -3.0f };

			if (pLocal->m_fFlags() & FL_DUCKING)
				vOffset.z = 8.0f;

			H::AimUtils->GetProjectileFireSetup(target.AngleTo, vOffset, &vLocalPos);

			break;
		}

		default: break;
	}

	if (m_CurProjInfo.GravityMod != 0.f)
	{
		return CanArcReach(vFrom, vTo, target.AngleTo, flTargetTime, target.Entity);
	}

	if (m_CurProjInfo.Flamethrower)
	{
		return H::AimUtils->TraceFlames(target.Entity, vLocalPos, vTo);
	}
	return H::AimUtils->TraceProjectile(target.Entity, vLocalPos, vTo);
}

bool CAimbotProjectile::SolveTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, const CUserCmd* pCmd, ProjTarget_t& target)
{
	Vec3 vLocalPos = pLocal->GetShootPos();

	if (m_CurProjInfo.Pipes)
	{
		const Vec3 vOffset = { 16.0f, 8.0f, -6.0f };
		H::AimUtils->GetProjectileFireSetup(pCmd->viewangles, vOffset, &vLocalPos);
	}

	m_TargetPath.clear();

	if (target.Entity->GetClassId() == ETFClassIds::CTFPlayer)
	{
		const auto pPlayer = target.Entity->As<C_TFPlayer>();

		const bool bDucked = pPlayer->m_fFlags() & FL_DUCKING;
		const bool bOnGround = pPlayer->m_fFlags() & FL_ONGROUND;

		if (!F::MovementSimulation->Initialize(pPlayer))
			return false;

		for (int nTick = 0; nTick < TIME_TO_TICKS(CFG::Aimbot_Projectile_Max_Simulation_Time); nTick++)
		{
			m_TargetPath.push_back(F::MovementSimulation->GetOrigin());

			F::MovementSimulation->RunTick(TICKS_TO_TIME(nTick));

			Vec3 vTarget = F::MovementSimulation->GetOrigin();

			OffsetPlayerPosition(pWeapon, vTarget, pPlayer, bDucked, bOnGround);

			float flTimeToTarget = 0.0f;

			if (!CalcProjAngle(vLocalPos, vTarget, target.AngleTo, flTimeToTarget))
				continue;

			target.TimeToTarget = flTimeToTarget;

			int nTargetTick = TIME_TO_TICKS(flTimeToTarget + SDKUtils::GetLatency());

			if (pWeapon->GetWeaponID() == TF_WEAPON_PIPEBOMBLAUNCHER)
			{
				const auto sticky_arm_time{ SDKUtils::AttribHookValue(0.8f, "sticky_arm_time", pLocal) };
				if (TICKS_TO_TIME(nTargetTick) < sticky_arm_time)
				{
					nTargetTick += TIME_TO_TICKS(fabsf(flTimeToTarget - sticky_arm_time));
				}
			}

			if ((nTargetTick == nTick || nTargetTick == nTick - 1))
			{
				auto runSplash = [&]()
					{
						auto isRocketLauncher{ pWeapon->GetWeaponID() == TF_WEAPON_ROCKETLAUNCHER };
						auto isDirectHit{ pWeapon->GetWeaponID() == TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT };
						auto isAirStrike{ pWeapon->m_iItemDefinitionIndex() == Soldier_m_TheAirStrike };

						if (!isRocketLauncher && !isDirectHit && !isAirStrike)
						{
							return false;
						}

						Vec3 mins{ target.Entity->m_vecMins() };
						Vec3 maxs{ target.Entity->m_vecMaxs() };

						auto center{ F::MovementSimulation->GetOrigin() + Vec3(0.0f, 0.0f, (mins.z + maxs.z) * 0.5f) };

						auto numPoints{ 80 };
						auto radius{ isRocketLauncher ? 180.0f : 80.0f };

						if (isAirStrike)
						{
							radius = 130.0f;
						}

						std::vector<Vec3> potential{};

						// we are getting fREAKY
						for (int n = 0; n < numPoints; n++)
						{
							auto a1{ acosf(1.0f - 2.0f * (static_cast<float>(n) / static_cast<float>(numPoints))) };
							auto a2{ (static_cast<float>(PI) * (3.0f - sqrtf(5.0f))) * static_cast<float>(n) };

							auto point{ center + Vec3{ sinf(a1) * cosf(a2), sinf(a1) * sinf(a2), cosf(a1) }.Scale(radius) };

							CTraceFilterWorldCustom filter{};
							trace_t trace{};

							H::AimUtils->Trace(center, point, MASK_SOLID, &filter, &trace);

							if (trace.fraction > 0.99f)
							{
								continue;
							}

							potential.push_back(trace.endpos);
						}

						std::ranges::sort(potential, [&](const Vec3& a, const Vec3& b)
							{
								return a.DistTo(F::MovementSimulation->GetOrigin()) < b.DistTo(F::MovementSimulation->GetOrigin());
							});

						for (auto& point : potential)
						{
							if (!CalcProjAngle(vLocalPos, point, target.AngleTo, target.TimeToTarget))
							{
								continue;
							}

							trace_t trace = {};
							CTraceFilterWorldCustom filter = {};

							H::AimUtils->TraceHull
							(
								GetOffsetShootPos(pLocal, pWeapon, pCmd),
								point,
								{ -4.0f, -4.0f, -4.0f },
								{ 4.0f, 4.0f, 4.0f },
								MASK_SOLID,
								&filter,
								&trace
							);

							if (trace.fraction < 0.9f || trace.startsolid || trace.allsolid)
							{
								continue;
							}

							H::AimUtils->Trace(trace.endpos, point, MASK_SOLID, &filter, &trace);

							if (trace.fraction < 1.0f)
							{
								continue;
							}

							bool splashDetected = CAimbotProjectile::NeuralNetworkSplashPrediction(point);
							if (splashDetected)
							{
								return true;
							}
						}

						return false;
					};

				if (CFG::Aimbot_Projectile_Rocket_Splash == 2 && runSplash())
				{
					F::MovementSimulation->Restore();

					return true;
				}

				if (CanSee(pLocal, pWeapon, vLocalPos, vTarget, target, flTimeToTarget))
				{
					F::MovementSimulation->Restore();

					return true;
				}
			}
		}

		F::MovementSimulation->Restore();
	}

	else
	{
		const Vec3 vTarget = target.Position;

		float flTimeToTarget = 0.0f;

		auto runSplash = [&]()
			{
				const auto isRocketLauncher{ pWeapon->GetWeaponID() == TF_WEAPON_ROCKETLAUNCHER };
				const auto isDirectHit{ pWeapon->GetWeaponID() == TF_WEAPON_ROCKETLAUNCHER_DIRECTHIT };
				const auto isAirStrike{ pWeapon->m_iItemDefinitionIndex() == Soldier_m_TheAirStrike };

				if (!isRocketLauncher && !isDirectHit && !isAirStrike)
				{
					return false;
				}

				const auto center{ target.Entity->GetCenter() };

				constexpr auto numPoints{ 80 };
				auto radius{ isRocketLauncher ? 150.0f : 70.0f };

				if (isAirStrike)
				{
					radius = 130.0f;
				}

				std::vector<Vec3> potential{};

				for (int n = 0; n < numPoints; n++)
				{
					const auto a1{ acosf(1.0f - 2.0f * (static_cast<float>(n) / static_cast<float>(numPoints))) };
					const auto a2{ (static_cast<float>(PI) * (3.0f - sqrtf(5.0f))) * static_cast<float>(n) };

					auto point{ center + Vec3{ sinf(a1) * cosf(a2), sinf(a1) * sinf(a2), cosf(a1) }.Scale(radius) };

					CTraceFilterWorldCustom filter{};
					trace_t trace{};

					H::AimUtils->Trace(center, point, MASK_SOLID, &filter, &trace);

					if (trace.fraction > 0.99f)
					{
						continue;
					}

					potential.push_back(trace.endpos);
				}

				std::ranges::sort(potential, [&](const Vec3& a, const Vec3& b)
					{
						return a.DistTo(center) < b.DistTo(center);
					});

				for (auto& point : potential)
				{
					if (!CalcProjAngle(vLocalPos, point, target.AngleTo, flTimeToTarget))
					{
						continue;
					}

					trace_t trace = {};
					CTraceFilterWorldCustom filter = {};

					H::AimUtils->TraceHull
					(
						GetOffsetShootPos(pLocal, pWeapon, pCmd),
						point,
						{ -4.0f, -4.0f, -4.0f },
						{ 4.0f, 4.0f, 4.0f },
						MASK_SOLID,
						&filter,
						&trace
					);

					if (trace.fraction < 0.9f || trace.startsolid || trace.allsolid)
					{
						continue;
					}

					H::AimUtils->Trace(trace.endpos, point, MASK_SOLID, &filter, &trace);

					if (trace.fraction < 1.0f)
					{
						continue;
					}

					bool splashDetected = CAimbotProjectile::NeuralNetworkSplashPrediction(point);
					if (splashDetected)
					{
						return true;
					}
				}

				return false;
			};

		if (CFG::Aimbot_Projectile_Rocket_Splash == 2 && runSplash())
		{
			return true;
		}

		if (CanSee(pLocal, pWeapon, vLocalPos, vTarget, target, flTimeToTarget))
		{
			return true;
		}
	}

	return false;
}


bool CAimbotProjectile::NeuralNetworkSplashPrediction(const Vec3& impactPoint)
{
	const auto pLocal = H::Entities->GetLocal();  

	if (!pLocal) return false;

	// step 1: gather features from the game state (such as distance from the target, projectile speed, etc.)
	Vec3 playerPosition = pLocal->GetAbsOrigin();  
	float distanceToImpact = impactPoint.DistTo(playerPosition);

	float projectileSpeed = 250.0f;  
	float playerVelocity = pLocal->m_vecVelocity().Length();  

	// step 2: normalize the features to be between 0 and 1 (this could be adjusted based on expected ranges)
	float normalizedDistance = std::min(distanceToImpact / 1000.0f, 1.0f);  // max distance normalized to 1
	float normalizedSpeed = std::min(projectileSpeed / 500.0f, 1.0f);  // max speed normalized to 1
	float normalizedVelocity = std::min(playerVelocity / 300.0f, 1.0f);  // max velocity normalized to 1

	// step 3: neural Network Architecture
	// inputs: normalizedDistance, normalizedSpeed, normalizedVelocity
	float inputLayer[3] = { normalizedDistance, normalizedSpeed, normalizedVelocity };

	// hidden Layer
	const float hiddenLayerWeights[2][3] = {
		{0.2f, 0.3f, 0.5f},  // weights for first hidden neuron
		{0.4f, 0.1f, 0.2f}   // weights for second hidden neuron
	};
	const float hiddenLayerBias[2] = { 0.1f, -0.2f };  // bias for hidden neurons
	float hiddenLayerOutput[2];

	// TODO: optimize this
	// compute the hidden layer outputs using a sigmoid activation
	for (int i = 0; i < 2; ++i) {
		hiddenLayerOutput[i] = Math::Sigmoid(inputLayer[0] * hiddenLayerWeights[i][0] +
			inputLayer[1] * hiddenLayerWeights[i][1] +
			inputLayer[2] * hiddenLayerWeights[i][2] +
			hiddenLayerBias[i]);
	}

	// output Layer
	const float outputLayerWeights[2] = { 0.7f, 0.9f };  // weights from hidden to output layer
	const float outputLayerBias = 0.1f;               // bias for output neuron

	// compute the output using sigmoid activation
	float output = Math::Sigmoid(hiddenLayerOutput[0] * outputLayerWeights[0] +
		hiddenLayerOutput[1] * outputLayerWeights[1] +
		outputLayerBias);

	// step 4: define a threshold for splash prediction
	const float predictionThreshold = CFG::PredictionThreshold; // if the score is higher than this, a splash is predicted

	// step 5: make the decision based on the score
	if (output > predictionThreshold)
	{
		// if the output is above the threshold, return true (indicating a splash)
		return true;
	}

	// if the output is below the threshold, return false (no splash)
	return false;
}





bool CAimbotProjectile::GetTarget(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, const CUserCmd* pCmd, ProjTarget_t& outTarget)
{
	const Vec3 vLocalPos = pLocal->GetShootPos();
	const Vec3 vLocalAngles = I::EngineClient->GetViewAngles();

	m_vecTargets.clear();

	if (CFG::Aimbot_Target_Players)
	{
		const auto nGroup = pWeapon->GetWeaponID() == TF_WEAPON_CROSSBOW ? EEntGroup::PLAYERS_ALL : EEntGroup::PLAYERS_ENEMIES;

		for (const auto pEntity : H::Entities->GetGroup(nGroup))
		{
			if (!pEntity || pEntity == pLocal)
				continue;

			const auto pPlayer = pEntity->As<C_TFPlayer>();

			if (pPlayer->deadflag() || pPlayer->InCond(TF_COND_HALLOWEEN_GHOST_MODE))
				continue;

			if (pPlayer->m_iTeamNum() != pLocal->m_iTeamNum())
			{
				if (CFG::Aimbot_Ignore_Friends && pPlayer->IsPlayerOnSteamFriendsList())
					continue;

				if (CFG::Aimbot_Ignore_Invisible && pPlayer->IsInvisible())
					continue;

				if (CFG::Aimbot_Ignore_Invulnerable && pPlayer->IsInvulnerable())
					continue;

				if (CFG::Aimbot_Ignore_Taunting && pPlayer->InCond(TF_COND_TAUNTING))
					continue;
			}

			else
			{
				if (pWeapon->GetWeaponID() == TF_WEAPON_CROSSBOW)
				{
					if (pPlayer->m_iHealth() >= pPlayer->GetMaxHealth() || pPlayer->IsInvulnerable())
					{
						continue;
					}
				}
			}

			Vec3 vPos = pPlayer->GetCenter();
			Vec3 vAngleTo = Math::CalcAngle(vLocalPos, vPos);
			const float flFOVTo = CFG::Aimbot_Projectile_Sort == 0 ? Math::CalcFov(vLocalAngles, vAngleTo) : 0.0f;
			const float flDistTo = vLocalPos.DistTo(vPos);

			if (CFG::Aimbot_Projectile_Sort == 0 && flFOVTo > CFG::Aimbot_Projectile_FOV)
				continue;

			m_vecTargets.emplace_back(AimTarget_t { pPlayer, vPos, vAngleTo, flFOVTo, flDistTo });
		}
	}

	if (CFG::Aimbot_Target_Buildings)
	{
		const auto isRescueRanger{ pWeapon->GetWeaponID() == TF_WEAPON_SHOTGUN_BUILDING_RESCUE };

		for (const auto pEntity : H::Entities->GetGroup(isRescueRanger ? EEntGroup::BUILDINGS_ALL : EEntGroup::BUILDINGS_ENEMIES))
		{
			if (!pEntity)
				continue;

			const auto pBuilding = pEntity->As<C_BaseObject>();

			if (pBuilding->m_bPlacing())
				continue;

			if (isRescueRanger && pBuilding->m_iTeamNum() == pLocal->m_iTeamNum() && pBuilding->m_iHealth() >= pBuilding->m_iMaxHealth())
			{
				continue;
			}

			Vec3 vPos = pBuilding->GetCenter(); //fuck teleporters when aimed at with pipes lma

			/*if (pEntity->GetClassId() == ETFClassIds::CObjectTeleporter || pWeapon->GetWeaponID() == TF_WEAPON_PIPEBOMBLAUNCHER)
			{
				vPos = pBuilding->m_vecOrigin();
			}*/

			Vec3 vAngleTo = Math::CalcAngle(vLocalPos, vPos);
			const float flFOVTo = CFG::Aimbot_Projectile_Sort == 0 ? Math::CalcFov(vLocalAngles, vAngleTo) : 0.0f;
			const float flDistTo = vLocalPos.DistTo(vPos);

			if (CFG::Aimbot_Projectile_Sort == 0 && flFOVTo > CFG::Aimbot_Projectile_FOV)
				continue;

			m_vecTargets.emplace_back(AimTarget_t { pBuilding, vPos, vAngleTo, flFOVTo, flDistTo });
		}
	}

	if (m_vecTargets.empty())
		return false;

	// Sort by target priority
	F::AimbotCommon->Sort(m_vecTargets, CFG::Aimbot_Projectile_Sort);

	const auto maxTargets{ std::min(CFG::Aimbot_Projectile_Max_Processing_Targets, static_cast<int>(m_vecTargets.size())) };
	auto targetsScanned{ 0 };

	for (auto& target : m_vecTargets)
	{
		if (target.Position.DistTo(vLocalPos) > 400.0f && targetsScanned >= maxTargets)
		{
			continue;
		}

		if (!SolveTarget(pLocal, pWeapon, pCmd, target))
		{
			targetsScanned++;

			continue;
		}

		if (CFG::Aimbot_Projectile_Sort == 0 && Math::CalcFov(vLocalAngles, target.AngleTo) > CFG::Aimbot_Projectile_FOV)
		{
			continue;
		}

		outTarget = target;
		return true;
	}

	return false;
}



bool CAimbotProjectile::ShouldAim(const CUserCmd* pCmd, C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	if (CFG::Aimbot_Projectile_Aim_Type != 1 || !IsFiring(pCmd, pLocal, pWeapon) || !pWeapon->HasPrimaryAmmoForShot())
		return false;

	float hitChance = CFG::ProjectileHC; 
	if (hitChance < 0.0f || hitChance > 100.0f) // probably bad 
		return false; 

	float randomChance = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100.0f;

	return randomChance <= hitChance;
}



void CAimbotProjectile::Aim(CUserCmd* pCmd, C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, const Vec3& vAngles)
{
	Vec3 vAngleTo = vAngles - pLocal->m_vecPunchAngle();

	if (m_CurProjInfo.Pipes)
	{
		Vec3 vAngle = {}, vForward = {}, vUp = {};
		Math::AngleVectors(vAngleTo, &vForward, nullptr, &vUp);
		const Vec3 vVelocity = (vForward * m_CurProjInfo.Speed) - (vUp * 200.0f);
		Math::VectorAngles(vVelocity, vAngle);
		vAngleTo.x = vAngle.x;
	}

	Math::ClampAngles(vAngleTo);

	switch (CFG::Aimbot_Projectile_Aim_Type)
	{
		case 0:
		{
			pCmd->viewangles = vAngleTo;
			break;
		}

		case 1:
		{
			if (m_CurProjInfo.Flamethrower ? true : G::bCanPrimaryAttack)
			{
				H::AimUtils->FixMovement(pCmd, vAngleTo);

				pCmd->viewangles = vAngleTo;

				if (m_CurProjInfo.Flamethrower)
				{
					G::bSilentAngles = true;
				}

				else
				{
					G::bPSilentAngles = true;
				}
			}

			break;
		}

		default: break;
	}
}

bool CAimbotProjectile::ShouldFire(CUserCmd* pCmd, C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	if (!CFG::Aimbot_AutoShoot)
	{
		//fucking fuck
		if (pWeapon->GetWeaponID() == TF_WEAPON_FLAME_BALL && pLocal->m_flTankPressure() < 100.0f)
			pCmd->buttons &= ~IN_ATTACK;

		return false;
	}

	return true;
}

void CAimbotProjectile::ViewmodelFlipper(C_BaseEntity* pLocal, CUserCmd* pCmd)
{
	if (!CFG::ViewmodelFlipper)
		return;

	if (static_cast<int>(G::CurItemDefIndex) == static_cast<int>(EWeaponType::PROJECTILE)) // hacky
		return;


	static auto cl_flipviewmodels = I::CVar->FindVar("cl_flipviewmodels");
	static bool defaultValue = cl_flipviewmodels->GetBool();


	const auto aimTarget = I::ClientEntityList->GetClientEntity(G::nTargetIndexEarly);
	if (G::nTargetIndexEarly <= 0 || !aimTarget)
	{
		cl_flipviewmodels->SetValue(defaultValue);
		return;
	}

	const auto localAngles = I::EngineClient->GetViewAngles();

	Vec3 source = localAngles;  
	Vec3 destination = Vec3(pCmd->viewangles.y, 0, 0); 

	const auto aimAngles = Math::CalcAngle(source, destination);

	auto mod = [](float a, float n)
		{
			return a - std::floor(a / n) * n;
		};

	const auto angleDelta = mod((aimAngles.y - localAngles.y) + 180.f, 360.f) - 180.f;
	if (angleDelta < -5.f)
	{
		cl_flipviewmodels->SetValue(true);
	}
	else if (angleDelta > 5.f)
	{
		cl_flipviewmodels->SetValue(false);
	}
}


void CAimbotProjectile::HandleFire(CUserCmd* pCmd, C_TFWeaponBase* pWeapon, C_TFPlayer* pLocal, const ProjTarget_t& target)
{
	const bool bIsBazooka = pWeapon->m_iItemDefinitionIndex() == Soldier_m_TheBeggarsBazooka;
	if (!bIsBazooka && !pWeapon->HasPrimaryAmmoForShot())
		return;

	const int nWeaponID = pWeapon->GetWeaponID();
	if (nWeaponID == TF_WEAPON_COMPOUND_BOW || nWeaponID == TF_WEAPON_PIPEBOMBLAUNCHER)
	{
		if (pWeapon->As<C_TFPipebombLauncher>()->m_flChargeBeginTime() > 0.0f)
			pCmd->buttons &= ~IN_ATTACK;

		else pCmd->buttons |= IN_ATTACK;
	}

	else if (nWeaponID == TF_WEAPON_CANNON)
	{
		if (CFG::Aimbot_Projectile_Auto_Double_Donk)
		{
			const float flDetonateTime = pWeapon->As<C_TFGrenadeLauncher>()->m_flDetonateTime();
			const float flDetonateMaxTime = SDKUtils::AttribHookValue(0.0f, "grenade_launcher_mortar_mode", pWeapon);
			float flCharge = Math::RemapValClamped(flDetonateTime - I::GlobalVars->curtime, 0.0f, flDetonateMaxTime, 0.0f, 1.0f);

			if (I::GlobalVars->curtime > flDetonateTime)
				flCharge = 1.0f;

			if (flCharge < target.TimeToTarget * 0.8f)
				pCmd->buttons &= ~IN_ATTACK;

			else pCmd->buttons |= IN_ATTACK;
		}

		else
		{
			if (pWeapon->As<C_TFGrenadeLauncher>()->m_flDetonateTime() > 0.0f)
				pCmd->buttons &= ~IN_ATTACK;

			else pCmd->buttons |= IN_ATTACK;
		}
	}

	else if (nWeaponID == TF_WEAPON_FLAME_BALL)
	{
		if (pLocal->m_flTankPressure() >= 100.0f)
			pCmd->buttons |= IN_ATTACK;
		else
			pCmd->buttons &= ~IN_ATTACK;
	}

	else
	{
		pCmd->buttons |= IN_ATTACK;
	}

	if (bIsBazooka && pWeapon->HasPrimaryAmmoForShot())
		pCmd->buttons &= ~IN_ATTACK;
}

bool CAimbotProjectile::IsFiring(const CUserCmd* pCmd, C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	if (!pWeapon->HasPrimaryAmmoForShot())
		return false;

	const int nWeaponID = pWeapon->GetWeaponID();
	if (nWeaponID == TF_WEAPON_COMPOUND_BOW || nWeaponID == TF_WEAPON_PIPEBOMBLAUNCHER || nWeaponID == TF_WEAPON_CANNON)
	{
		return (G::nOldButtons & IN_ATTACK) && !(pCmd->buttons & IN_ATTACK);
	}

	if (nWeaponID == TF_WEAPON_FLAME_BALL)
	{
		return pLocal->m_flTankPressure() >= 100.0f && (pCmd->buttons & IN_ATTACK);
	}

	if (pWeapon->m_iItemDefinitionIndex() == Soldier_m_TheBeggarsBazooka)
		return G::bCanPrimaryAttack;

	if (nWeaponID == TF_WEAPON_FLAMETHROWER)
	{
		return pCmd->buttons & IN_ATTACK;
	}

	return (pCmd->buttons & IN_ATTACK) && G::bCanPrimaryAttack;
}

void CAimbotProjectile::Run(CUserCmd* pCmd, C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon)
{
	if (!CFG::Aimbot_Projectile_Active)
		return;

	if (!GetProjectileInfo(pWeapon))
		return;

	if (CFG::Aimbot_Projectile_Sort == 0)
		G::flAimbotFOV = CFG::Aimbot_Projectile_FOV;

	if (Shifting::bShifting && !Shifting::bShiftingWarp)
		return;

	if (!H::Input->IsDown(CFG::Aimbot_Key))
		return;

	ProjTarget_t target = {};
	if (GetTarget(pLocal, pWeapon, pCmd, target) && target.Entity)
	{
		G::nTargetIndexEarly = target.Entity->entindex();

		G::nTargetIndex = target.Entity->entindex();

		if (ShouldFire(pCmd, pLocal, pWeapon))
			HandleFire(pCmd, pWeapon, pLocal, target);

		const bool bIsFiring = IsFiring(pCmd, pLocal, pWeapon);

		G::bFiring = bIsFiring;

		if (ShouldAim(pCmd, pLocal, pWeapon) || bIsFiring)
		{
			/*const Vec3 ang_center{ Math::CalcAngle(pLocal->GetShootPos(), Target.m_vPosition) };
			const Vec3 ang_offset{ Math::CalcAngle(getOffsetShootPos(pLocal, pWeapon, pCmd), Target.m_vPosition)};

			const float correction_scale{ Math::RemapValClamped(pLocal->GetShootPos().DistTo(Target.m_vPosition), 0.0f, 19202.0f, 0.0f, 1.0f) };

			const float base_val{ pWeapon->GetWeaponID() == TF_WEAPON_COMPOUND_BOW ? 5.3f : 6.5f };

			const Vec3 correction{ (ang_offset - ang_center) * (base_val * correction_scale) };

			Target.m_vAngleTo -= correction;*/

			Aim(pCmd, pLocal, pWeapon, target.AngleTo);

			if (bIsFiring && m_TargetPath.size() > 1)
			{
				I::DebugOverlay->ClearAllOverlays();

				//drawProjPath(pCmd, Target.TimeToTarget);
				DrawMovePath(m_TargetPath);
				m_TargetPath.clear();
			}
		}
	}
}
