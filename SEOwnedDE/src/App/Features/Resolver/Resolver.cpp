#include "Resolver.h"

#include "../src/SDK/TF2/igameevents.h"

#include "vector"
#include "../../../Utils/Math/Math.h"
#include "../src/SDK/TF2/c_baseplayer.h"
#include "../src/SDK/TF2/c_tf_player.h"
#include "../src/App/Features/CFG.h"

#include "../src/Utils/Utils.h"
#include "../src/SDK/SDK.h"


void CResolver::OnShot(CUserCmd* cmd, C_TFPlayer* player) {
    if (CFG::Resolver && player) {
        Vector currentAngles = player->GetAbsAngles();

        AntiAimPattern pattern = DetectAntiAimPattern(player);

        Vector newAngles = AdjustAnglesBasedOnPattern(currentAngles, pattern);

        newAngles = PitchResolver(newAngles, pitch::down);

        if (CFG::ResolverClamp) {
            Math::ClampAngles(newAngles);
        }

        player->SetAbsAngles(newAngles);
    }
}

AntiAimPattern CResolver::DetectAntiAimPattern(C_TFPlayer* player) {
    return AntiAimPattern::Edge;
}

Vector CResolver::AdjustAnglesBasedOnPattern(const Vector& angles, AntiAimPattern pattern) {
    Vector newAngles = angles;

    if (CFG::Resolver_Yaw) {
        switch (pattern) {
        case AntiAimPattern::Edge:
            newAngles.y = 90.0f; 
            break;
        case AntiAimPattern::Jitter:
            newAngles.y = 45.0f;  
            break;
        case AntiAimPattern::left:
            newAngles.y = 89.0f;  
            break;
        case AntiAimPattern::Right:
            newAngles.y = 125.0f;  
            break;
        case AntiAimPattern::Backwards:
            newAngles.y = 80.0f;  
            break;
        default:
            break;
        }
    }

    return newAngles; 
}

Vector CResolver::PitchResolver(const Vector& angles, pitch pattern) {
    Vector newAngles = angles; 

    if (CFG::Resolver_Pitch) {
        switch (pattern) {
        case pitch::up:
            newAngles.x = -89.0f; 
            break;
        case pitch::down:
            newAngles.x = 89.0f; 
            break;
        default:
            break;
        }
    }

    return newAngles; 
}

void CResolver::OnPlayerHurt(IGameEvent* pEvent)
{
    const bool bLocal = I::EngineClient->GetPlayerForUserID(pEvent->GetInt("attacker")) == I::EngineClient->GetLocalPlayer();
    if (!bLocal)
        return;

    auto pVictim = I::ClientEntityList->GetClientEntity(I::EngineClient->GetPlayerForUserID(pEvent->GetInt("userid")));

    if (pVictim)
    {
        if (G::bCanHeadshot)
        {	// should be headshot
            const bool bCrit = pEvent->GetBool("crit");
            if (!bCrit)
                return;
        }
    }
    return;
}
