#include "Crits.h"

#include "../CFG.h"

bool IsFiring(const CUserCmd* pCmd, C_TFWeaponBase* weapon)
{
	if (weapon->GetSlot() != 2)
	{
		if (!weapon->HasPrimaryAmmoForShot())
			return false;

		const int nWeaponID = weapon->GetWeaponID();

		if (nWeaponID == TF_WEAPON_PIPEBOMBLAUNCHER || nWeaponID == TF_WEAPON_CANNON)
			return (G::nOldButtons & IN_ATTACK) && !(pCmd->buttons & IN_ATTACK);

		if (weapon->m_iItemDefinitionIndex() == Soldier_m_TheBeggarsBazooka)
			return G::bCanPrimaryAttack;
	}

	return (pCmd->buttons & IN_ATTACK) && G::bCanPrimaryAttack;
}

int FindCritCmd(const CUserCmd* pCmd, C_TFWeaponBase* pWeapon, bool bCrit)
{
	const int nBackupRandomSeed = *SDKUtils::RandomSeed();
	int nCommandNumber = pCmd->command_number;
	int nCritCommand = nCommandNumber;

	for (int n = 0; n < 4096; n++)
	{
		*SDKUtils::RandomSeed() = MD5_PseudoRandom(nCommandNumber) & 0x7FFFFFFF;

		bool bCalc = false;

		if (pWeapon->GetSlot() == 2)
		{
			bCalc = pWeapon->CalcIsAttackCriticalHelperMelee();
		}
		else
		{
			bCalc = pWeapon->CalcIsAttackCriticalHelper();
		}

		if (bCrit ? bCalc : !bCalc)
		{
			nCritCommand = nCommandNumber;
			break;
		}

		nCommandNumber++;
	}

	*SDKUtils::RandomSeed() = nBackupRandomSeed;
	return nCritCommand;
}

void CCrits::Run(CUserCmd* pCmd)
{
	const auto pLocal = H::Entities->GetLocal();

	if (!pLocal || pLocal->deadflag() || pLocal->IsCritBoosted() || pLocal->IsMiniCritBoosted())
		return;

	static auto tf_weapon_criticals = I::CVar->FindVar("tf_weapon_criticals");

	if (!tf_weapon_criticals->GetInt())
	{
		return;
	}

	const auto pWeapon = H::Entities->GetWeapon();

	if (!pWeapon || pWeapon->GetWeaponID() == TF_WEAPON_KNIFE || !IsFiring(pCmd, pWeapon))
		return;

	// Melee crits
	if (pWeapon->GetSlot() == 2)
	{
		static auto tf_weapon_criticals_melee = I::CVar->FindVar("tf_weapon_criticals_melee");

		if (!tf_weapon_criticals_melee->GetInt())
		{
			return;
		}

		if (H::Input->IsDown(CFG::Exploits_Crits_Force_Crit_Key_Melee))
		{
			bool wantCrit = true;

			// Check if the aim target should be critted
			if (G::nTargetIndex)
			{
				const auto ent = I::ClientEntityList->GetClientEntity(G::nTargetIndex);

				if (ent && ent->GetClassId() == ETFClassIds::CTFPlayer && ent->As<C_TFPlayer>()->m_iTeamNum() == pLocal->m_iTeamNum())
				{
					wantCrit = false;
				}
			}

			if (wantCrit)
			{
				pCmd->command_number = FindCritCmd(pCmd, pWeapon, true);
			}
			else
			{
				if (CFG::Exploits_Crits_Skip_Random_Crits)
				{
					pCmd->command_number = FindCritCmd(pCmd, pWeapon, false);
				}
			}
		}
		else
		{
			if (CFG::Exploits_Crits_Skip_Random_Crits)
			{
				pCmd->command_number = FindCritCmd(pCmd, pWeapon, false);
			}
		}
	}

	// Hitscan crits
	else
	{
		if (H::Input->IsDown(CFG::Exploits_Crits_Force_Crit_Key))
		{
			pCmd->command_number = FindCritCmd(pCmd, pWeapon, true);
		}
		else
		{
			if (CFG::Exploits_Crits_Skip_Random_Crits)
			{
				pCmd->command_number = FindCritCmd(pCmd, pWeapon, false);
			}
		}
	}
}


void CCrits::Indicator()
{
	if (!CFG::CritIndicator)
		return;

	if (CFG::Misc_Clean_Screenshot && I::EngineClient->IsTakingScreenshot())
		return;

	if (I::EngineVGui->IsGameUIVisible() || SDKUtils::BInEndOfMatch())
		return;

	const auto pLocal = H::Entities->GetLocal();

	if (!pLocal || pLocal->deadflag())
		return;

	const auto pWeapon = H::Entities->GetWeapon();

	if (!pWeapon)
		return;

	static int nBarW = 80;
	static int nBarH = 4;

	const int nBarX = (H::Draw->GetScreenW() / 2) - (nBarW / 2);
	const int nBarY = (H::Draw->GetScreenH() / 2) + 150; // ShiftBar Y position
	const int textY = nBarY - 12; 
	const int jew = nBarY - 8;// Text Y position for ShiftBar

	// Get Bucket value
	static auto bucket = I::CVar->FindVar("tf_weapon_criticals_bucket_cap");

	// Define colors
	const Color_t color = CFG::Menu_Accent_Secondary; // Color for the bar fill
	const Color_t colorDim = { color.r, color.g, color.b, 25 }; // Dimmed color for the background fill

	// Draw the Shifting indicator
	if (CFG::Exploits_Shifting_Indicator_Style == 0)
	{
		H::Draw->Rect(nBarX - 1, nBarY - 1, nBarW + 2, nBarH + 2, CFG::Menu_Background);

		if (Shifting::nAvailableTicks > 0)
		{
			const int nFillWidth = static_cast<int>(Math::RemapValClamped(
				static_cast<float>(Shifting::nAvailableTicks),
				0.0f, static_cast<float>(MAX_COMMANDS),
				0.0f, static_cast<float>(nBarW)
			));

			H::Draw->GradientRect(nBarX, nBarY, nFillWidth, nBarH, colorDim, color, false);
			H::Draw->OutlinedRect(nBarX, nBarY, nFillWidth, nBarH, color);
		}

		// Draw Bucket value in the bar
		if (bucket) // Check if bucket is valid
		{
			int bucketValue = std::min(static_cast<int>(bucket->GetInt()), 1000); // Cap at 1000
			const int bucketFillWidth = static_cast<int>(Math::RemapValClamped(
				static_cast<float>(bucketValue),
				0.0f, 1000.0f,
				0.0f, static_cast<float>(nBarW)
			));

			// Draw Bucket value within the ShiftBar
			H::Draw->GradientRect(nBarX, nBarY, bucketFillWidth, nBarH, colorDim, color, false);
			H::Draw->OutlinedRect(nBarX, nBarY, bucketFillWidth, nBarH, color);

			char bucketText[32];
			snprintf(bucketText, sizeof(bucketText), "%d", bucketValue);
			H::Draw->String(H::Fonts->Get(EFonts::ESP_SMALL), nBarX + (bucketFillWidth / 2) - 10, textY, color, 0, bucketText); // Center the Bucket text

			// Check if crit banned and render "Crit Banned" text in the same position
			if (!pLocal->m_iCritMult())
			{
				const char* critBannedText = "Crit Banned";
				H::Draw->String(H::Fonts->Get(EFonts::ESP_SMALL), nBarX + (bucketFillWidth / 2) - 23, jew + 15, color, 0, critBannedText); // Adjusted Y position
			}
		}
	}

	if (CFG::Exploits_Shifting_Indicator_Style == 1)
	{
		const float end{ Math::RemapValClamped(static_cast<float>(Shifting::nAvailableTicks), 0.0f, MAX_COMMANDS, -90.0f, 359.0f) };

		H::Draw->Arc(nBarX + nBarW / 2, nBarY, 21, 6.0f, -90.0f, 359.0f, CFG::Menu_Background);
		H::Draw->Arc(nBarX + nBarW / 2, nBarY, 20, 4.0f, -90.0f, end, CFG::Menu_Accent_Secondary);
		H::Draw->String(H::Fonts->Get(EFonts::ESP_SMALL), nBarX, textY, CFG::Menu_Accent_Secondary, 0, std::to_wstring(Shifting::nAvailableTicks).c_str());
	}

	// The crit banned message rendering is handled in the bucket section now.
}
