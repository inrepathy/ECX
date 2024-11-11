#include "Aimbot.h"

#include "AimbotHitscan/AimbotHitscan.h"
#include "AimbotProjectile/AimbotProjectile.h"
#include "AimbotMelee/AimbotMelee.h"
#include "../src/App/Features/Resolver/Resolver.h"

#include "../CFG.h"

void CAimbot::RunMain(CUserCmd* pCmd)
{
	G::nTargetIndex = -1;
	G::flAimbotFOV = 0.0f;
	G::nTargetIndexEarly = -1;

	if (!CFG::Aimbot_Active || I::EngineVGui->IsGameUIVisible() || I::MatSystemSurface->IsCursorVisible() || SDKUtils::BInEndOfMatch())
		return;

	if (Shifting::bRecharging)
		return;

	const auto pLocal = H::Entities->GetLocal();
	const auto pWeapon = H::Entities->GetWeapon();

	if (!pLocal || !pWeapon
		|| pLocal->deadflag()
		|| pLocal->InCond(TF_COND_TAUNTING) || pLocal->InCond(TF_COND_PHASE)
		|| pLocal->InCond(TF_COND_HALLOWEEN_GHOST_MODE)
		|| pLocal->InCond(TF_COND_HALLOWEEN_BOMB_HEAD)
		|| pLocal->InCond(TF_COND_HALLOWEEN_KART)
		|| pLocal->m_bFeignDeathReady() || pLocal->m_flInvisibility() > 0.0f
		|| pWeapon->m_iItemDefinitionIndex() == Soldier_m_RocketJumper || pWeapon->m_iItemDefinitionIndex() == Demoman_s_StickyJumper)
		return;

	switch (H::AimUtils->GetWeaponType(pWeapon))
	{
		case EWeaponType::HITSCAN:
		{
			F::AimbotHitscan->Run(pCmd, pLocal, pWeapon);
			break;

		//if (CFG::Resolver) {
		//		F::Resolver->OnShot(pLocal);
		//	}
		}

		case EWeaponType::PROJECTILE:
		{
			F::AimbotProjectile->Run(pCmd, pLocal, pWeapon);
			break;
		}


		case EWeaponType::MELEE:
		{
			F::AimbotMelee->Run(pCmd, pLocal, pWeapon);
			break;
		}

		default: break;
	}
}

bool IsHitboxValid(int nHitbox, int index, bool bStatic = false)
{
	const int iStatic = CFG::SafeHitbox;
	switch (nHitbox)
	{
	case -1: return true;
	case HITBOX_HEAD: return (index & (1 << 0) && !(!bStatic && iStatic & (1 << 0)));
	case HITBOX_NECK: return (index & (1 << 1) && !(!bStatic && iStatic & (1 << 1)));
	case HITBOX_PELVIS: return (index & (1 << 3) && !(!bStatic && iStatic & (1 << 2)));
	case HITBOX_BODY: return (index & (1 << 4) && !(!bStatic && iStatic & (1 << 3)));
	case HITBOX_CHEST: return (index & (1 << 6) && !(!bStatic && iStatic & (1 << 4)));
	}
	return false;
};



void CAimbot::Run(CUserCmd* pCmd)
{
	RunMain(pCmd);

	// Same-ish code below to see if we are firing manually
	const auto pLocal = H::Entities->GetLocal();
	const auto pWeapon = H::Entities->GetWeapon();

	if (!pLocal || !pWeapon
		|| pLocal->deadflag()
		|| pLocal->InCond(TF_COND_TAUNTING) || pLocal->InCond(TF_COND_PHASE)
		|| pLocal->m_bFeignDeathReady() || pLocal->m_flInvisibility() > 0.0f)
		return;

	if (CFG::Lagfix) {
		int twentytwo = 2;

		if (I::EngineClient->GetNetChannelInfo()->GetAvgChoke(twentytwo) > 2)
			return; // player is choking, don't run aimbot this tick
	}

	const auto nWeaponType = H::AimUtils->GetWeaponType(pWeapon);

	if (!G::bFiring)
	{
		switch (nWeaponType)
		{
		case EWeaponType::HITSCAN:
		{
			G::bFiring = F::AimbotHitscan->IsFiring(pCmd, pWeapon);
			break;
		}

		case EWeaponType::PROJECTILE:
		{
			G::bFiring = F::AimbotProjectile->IsFiring(pCmd, pLocal, pWeapon);
			break;
		}

		case EWeaponType::MELEE:
		{
			G::bFiring = F::AimbotMelee->IsFiring(pCmd, pWeapon);
			break;
		}

		default: break;
		}
	}


	if (CFG::AutoSecondarySwitch && pWeapon->m_iClip1() < 1 && pWeapon->GetSlot() == SLOT_PRIMARY && G::CurItemDefIndex != TF_WEAPON_SNIPERRIFLE
		&& G::CurItemDefIndex != TF_WEAPON_SNIPERRIFLE_DECAP && G::CurItemDefIndex != TF_WEAPON_SNIPERRIFLE_CLASSIC
		&& pWeapon->m_iItemDefinitionIndex() != Engi_m_TheWidowmaker && pWeapon->m_iItemDefinitionIndex() != Sniper_m_TheMachina && ((GetAsyncKeyState(CFG::Aimbot_Key) & 0x8000) && H::Input->IsGameFocused()))
	{
		I::EngineClient->ClientCmd_Unrestricted("slot2");
	}

	// Projectile NoSpread
	if (G::bFiring && nWeaponType == EWeaponType::PROJECTILE && CFG::Aimbot_Projectile_NoSpread)
	{
		SDKUtils::SharedRandomInt("SelectWeightedSequence", 0, 0, pCmd->random_seed);

		for (int i = 0; i < 6; ++i)
		{
			I::UniformRandomStream->RandomFloat();
		}

		switch (pWeapon->GetWeaponID())
		{
		case TF_WEAPON_GRENADELAUNCHER:
		case TF_WEAPON_PIPEBOMBLAUNCHER:
		case TF_WEAPON_CANNON:
		{
			pCmd->command_number = SDKUtils::FindCmdNumWithSeed(pCmd->command_number, 39);
			break;
		}

		case TF_WEAPON_SYRINGEGUN_MEDIC:
		{
			pCmd->viewangles.x -= I::UniformRandomStream->RandomFloat(-1.5f, 1.5f);
			pCmd->viewangles.y -= I::UniformRandomStream->RandomFloat(-1.5f, 1.5f);
			Math::ClampAngles(pCmd->viewangles);
			G::bPSilentAngles = true;
			break;
		}

		case TF_WEAPON_COMPOUND_BOW:
		{
			Vec3 vSpread = {}, vSrc = {};
			pWeapon->GetProjectileFireSetup(pLocal, { 0.0f, 0.0f, 0.0f }, &vSrc, &vSpread, false, 2000.0f);
			pCmd->viewangles -= (vSpread - I::EngineClient->GetViewAngles());
			Math::ClampAngles(pCmd->viewangles);
			G::bPSilentAngles = true;
			break;
		}

		default:
		{
			if (pWeapon->m_iItemDefinitionIndex() == Soldier_m_TheBeggarsBazooka)
			{
				Vec3 vSpread = {};
				pWeapon->GetSpreadAngles(vSpread);
				pCmd->viewangles -= (vSpread - I::EngineClient->GetViewAngles());
				Math::ClampAngles(pCmd->viewangles);
				G::bPSilentAngles = true;
			}

			break;
		}
		}
	}
}
