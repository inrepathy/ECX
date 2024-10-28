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

class CResolver {
public:
    const bool bCrit = ("crit");

  //  void ResolveAngles(C_TFPlayer* player);
    void OnShot(CUserCmd* cmd, C_TFPlayer* player); 
    void OnPlayerHurt(IGameEvent* pEvent);

private:
    AntiAimPattern DetectAntiAimPattern(C_TFPlayer* player);
    Vector AdjustAnglesBasedOnPattern(const Vector& angles, AntiAimPattern pattern);
    Vector PitchResolver(const Vector& angles, pitch pattern);
    Vector AdjustAnglesForNonHeadshot(const Vector& angles);

    bool lastShotHeadshot = false;
public:
    CResolver() = default;
};

MAKE_SINGLETON_SCOPED(CResolver, Resolver, F)

#endif 

//self coderz shit mafia gang bang
