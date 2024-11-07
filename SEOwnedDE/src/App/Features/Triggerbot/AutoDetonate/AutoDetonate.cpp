#include "AutoDetonate.h"
#include "../../CFG.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void CAutoDetonate::Run(C_TFPlayer* pLocal, C_TFWeaponBase* pWeapon, CUserCmd* pCmd)
{
    if (!CFG::Triggerbot_AutoDetonate_Active)
        return;

    if (pLocal->m_iClass() != TF_CLASS_DEMOMAN)
        return;

    for (const auto pEntity : H::Entities->GetGroup(EEntGroup::PROJECTILES_LOCAL_STICKIES))
    {
        if (!pEntity)
            continue;

        const auto pSticky = pEntity->As<C_TFGrenadePipebombProjectile>();

        if (!pSticky || pSticky->m_iType() == TF_GL_MODE_REMOTE_DETONATE_PRACTICE)
            continue;

        if (pSticky->IsDormant() || !pSticky->IsInValidTeam())
            continue;

        if (I::GlobalVars->curtime < pSticky->m_flCreationTime() + SDKUtils::AttribHookValue(0.8f, "sticky_arm_time", pLocal))
            continue;

        const float flRadius = pSticky->m_bTouched() ? 150.0f : 100.0f;

        Vector center = pSticky->GetCenter();

        Color_t circleColor = { 255, 0, 0, 255 };
        const int numSegments = 20;

        for (int i = 0; i < numSegments; ++i)
        {

            if (CFG::DetonateColor_true) {
                float angle1 = (2.0f * M_PI * i) / numSegments;
                float angle2 = (2.0f * M_PI * (i + 1)) / numSegments;

                Vector point1(center.x + flRadius * cos(angle1), center.y + flRadius * sin(angle1), center.z);
                Vector point2(center.x + flRadius * cos(angle2), center.y + flRadius * sin(angle2), center.z);


                // todo: do this with H::Draw->
                I::DebugOverlay->AddLineOverlay(point1, point2, CFG::DetonateColor.r, CFG::DetonateColor.g, CFG::DetonateColor.b, false, 0.1f);
            }
        }


        if (CFG::Triggerbot_AutoDetonate_Target_Players)
        {
            for (const auto pPlayerEntity : H::Entities->GetGroup(EEntGroup::PLAYERS_ENEMIES))
            {
                if (!pPlayerEntity)
                    continue;

                const auto pPlayer = pPlayerEntity->As<C_TFPlayer>();

                if (pPlayer->deadflag() || pPlayer->InCond(TF_COND_HALLOWEEN_GHOST_MODE))
                    continue;

                if (CFG::Triggerbot_AutoDetonate_Ignore_Friends && pPlayer->IsPlayerOnSteamFriendsList())
                    continue;

                if (CFG::Triggerbot_AutoDetonate_Ignore_Invisible && pPlayer->IsInvisible())
                    continue;

                if (CFG::Triggerbot_AutoDetonate_Ignore_Invulnerable && pPlayer->IsInvulnerable())
                    continue;

                if (pSticky->GetCenter().DistTo(pPlayer->GetCenter()) < flRadius)
                {
                    if (H::AimUtils->TraceEntityAutoDet(pPlayer, pSticky->GetCenter(), pPlayer->GetCenter()))
                    {
                        if (pSticky->m_bDefensiveBomb())
                        {
                            const Vec3 vAngle = Math::CalcAngle(pLocal->GetShootPos(), pSticky->GetCenter());
                            H::AimUtils->FixMovement(pCmd, vAngle);
                            pCmd->viewangles = vAngle;
                            G::bSilentAngles = true;
                        }

                        pCmd->buttons |= IN_ATTACK2;
                        return;
                    }
                }
            }
        }

        if (CFG::Triggerbot_AutoDetonate_Target_Buildings)
        {
            for (const auto pBuildingEntity : H::Entities->GetGroup(EEntGroup::BUILDINGS_ENEMIES))
            {
                if (!pBuildingEntity)
                    continue;

                if (pSticky->GetCenter().DistTo(pBuildingEntity->GetCenter()) < flRadius)
                {
                    if (H::AimUtils->TraceEntityAutoDet(pBuildingEntity, pSticky->GetCenter(), pBuildingEntity->GetCenter()))
                    {
                        if (pSticky->m_bDefensiveBomb())
                        {
                            const Vec3 vAngle = Math::CalcAngle(pLocal->GetShootPos(), pSticky->GetCenter());
                            H::AimUtils->FixMovement(pCmd, vAngle);
                            pCmd->viewangles = vAngle;
                            G::bSilentAngles = true;
                        }

                        pCmd->buttons |= IN_ATTACK2;
                        return;
                    }
                }
            }
        }
    }
}
