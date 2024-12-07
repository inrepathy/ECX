#include "Resolver.h"

#include "../src/SDK/TF2/igameevents.h"

#include "vector"
#include "../../../Utils/Math/Math.h"
#include "../src/SDK/TF2/c_baseplayer.h"
#include "../src/SDK/TF2/c_tf_player.h"
#include "../src/App/Features/CFG.h"

#include "../src/Utils/Utils.h"
#include "../src/SDK/SDK.h"


void CResolver::OnShot(CUserCmd* cmd, C_TFPlayer* player, int iIndex, const Vec3 vvAngles) {
   
    
    if (!CFG::Resolver && player)
        return;


    auto pEntity = I::ClientEntityList->GetClientEntity(iIndex);
    if (!pEntity)
        return;

    Vec3 vAngAdjusted = vvAngles;
    Vec3 vAngStore = vvAngles;

    if (!IsOnShotPitchReliable(vvAngles.x))
    {
        float flAdjustedPitch = vvAngles.x;

        vAngStore.x += 360.f;
        vAngStore.x *= -1;

        vAngAdjusted.x = GetRealPitch(flAdjustedPitch);

        if (fabsf(flAdjustedPitch) > 89.f)
            vAngStore.y += 180; // account for likely yaw faking
        while (vAngStore.y > 360)
            vAngStore.y -= 360.f; // hacky fix for previous line
        vAngStore.x += 540; // (360+180)
    }
    

   mResolverData[player].pLastFireAngles = { { I::GlobalVars->tickcount, true }, vAngStore };
    SetAngles(vAngAdjusted, player);

    /*if (SetvAngles(vAngAdjusted, player))
    {
        I::CVar->ConsoleColorPrintf({ 255, 70, 70, 255 }, "works\n");
    }
    else
    {
        I::CVar->ConsoleColorPrintf({ 255, 70, 70, 255 }, "nigger\n");
    }*/

}

void CResolver::SetAngles(const Vec3 vvAngles, C_TFPlayer* pEntity)
{
    if (auto pAnimState = pEntity->GetAnimState())
    {
        pEntity->m_angEyeAnglesX() = vvAngles.x;
        pAnimState->m_flCurrentFeetYaw = vvAngles.y;
        pAnimState->m_flGoalFeetYaw = vvAngles.y;
        pAnimState->Update(vvAngles.y, vvAngles.x);
    }
}

bool CResolver::IsOnShotPitchReliable(const float flPitch)
{
    if (flPitch > 180)
        return flPitch > 273.f;
    else
        return flPitch < 87.f;
}

float CResolver::GetRealPitch(const float flPitch)
{
    if (flPitch < 157.5f)
        return 89.f;
    else
        return -89.f;
}


Vector CResolver::AdjustAnglesBasedOnPattern(const Vec3 vAngles, AntiAimPattern pattern) {
    Vector newvAngles = vAngles;

    if (CFG::Resolver_Yaw) {
        switch (pattern) {
        case AntiAimPattern::Edge:
            newvAngles.y = 90.0f; 
            break;
        case AntiAimPattern::Jitter:
            newvAngles.y = 45.0f;  
            break;
        case AntiAimPattern::left:
            newvAngles.y = 89.0f;  
            break;
        case AntiAimPattern::Right:
            newvAngles.y = 125.0f;  
            break;
        case AntiAimPattern::Backwards:
            newvAngles.y = 80.0f;  
            break;
        default:
            break;
        }
    }

    return newvAngles; 
}

Vector CResolver::PitchResolver(const Vec3 vAngles, pitch pattern) {
    Vector newvAngles = vAngles; 

    if (CFG::Resolver_Pitch) {
        switch (pattern) {
        case pitch::up:
            newvAngles.x = -89.0f; 
            break;
        case pitch::down:
            newvAngles.x = 89.0f; 
            break;
        default:
            break;
        }
    }

    return newvAngles; 
}

void CResolver::CreateMove()
{
    if (I::GlobalVars->tickcount > pWaiting.first && pWaiting.second.first)
    {
        mResolverData[pWaiting.second.first].iYawIndex++;
        if (mResolverData[pWaiting.second.first].iYawIndex > 3)
            mResolverData[pWaiting.second.first].iYawIndex = 0;
        pWaiting = { 0, {nullptr, false} };
    }
}

void CResolver::OnPlayerHurt(IGameEvent* pEvent)
{
    if (I::EngineClient->GetPlayerForUserID(pEvent->GetInt("attacker")) != I::EngineClient->GetLocalPlayer())
        return;

    if (I::ClientEntityList->GetClientEntity(I::EngineClient->GetPlayerForUserID(pEvent->GetInt("userid"))) == pWaiting.second.first)
    {
        if (pWaiting.second.second && G::bCanHeadshot && !pEvent->GetBool("crit"))
            return;
        pWaiting = { 0, { nullptr, false } };
    }
}
