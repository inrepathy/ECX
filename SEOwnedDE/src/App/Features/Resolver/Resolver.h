#include "../../../SDK/TF2/c_tf_player.h"
#include "../src/SDK/TF2/igameevents.h"

#ifndef CRESOLVER_H
#define CRESOLVER_H

enum class AntiAimPattern {
    Edge,
    Jitter,
    left,
    Right,
    Backwards,
    forward, // Corrected spelling
};

enum class pitch {
    up,
    down,
};

struct ResolveData
{
    //config data
    bool bEnabled = false;			//	should we resolve this player

    //bruteforce data
    int iYawIndex = 0;

    //logical data
    std::pair<int, float> pLastSniperPitch = { 0, 0.f };
    float flPitchNoise = 0.f;		//	noise around sniper dot pitch
    int iPitchNoiseSteps = 0;

    //historical data
    std::pair<std::pair<int, bool>, Vec3> pLastFireAngles = { { 0, false }, {} };
    Vec2 vOriginalAngles = { 0.f, 0.f };
};

class CResolver {

    std::unordered_map<C_TFPlayer*, ResolveData> mResolverData;
    std::pair<int, std::pair<C_TFPlayer*, bool>> pWaiting = { 0, {nullptr, false} };

public:
    const bool bCrit = ("crit");

  //  void ResolveAngles(C_TFPlayer* player);
    void OnShot(CUserCmd* cmd, C_TFPlayer* player, int iIndex, const Vec3 vAngles);
    void OnPlayerHurt(IGameEvent* pEvent);
    bool IsOnShotPitchReliable(const float flPitch);
    float GetRealPitch(const float flPitch);
    void CreateMove();
    void SetAngles(const Vec3 vAngles, C_TFPlayer* pEntity);

private:
    AntiAimPattern DetectAntiAimPattern(C_TFPlayer* player);
    Vector AdjustAnglesBasedOnPattern(const Vec3 vAngles, AntiAimPattern pattern);
    Vector PitchResolver(const Vec3 vAngles, pitch pattern);
    Vector AdjustAnglesForNonHeadshot(const Vector& angles);

    bool lastShotHeadshot = false;
public:
    CResolver() = default;
};

MAKE_SINGLETON_SCOPED(CResolver, Resolver, F)

#endif 

//self coderz shit mafia gang bang
