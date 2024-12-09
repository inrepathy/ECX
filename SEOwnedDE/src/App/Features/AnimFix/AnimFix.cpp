#include "../../SEOwnedDE/src/SDK/SDK.h"

#include "AnimFix.h"


/*

  TODO: make this not pasted
  TODO: make it work on other players too probably

  i mean it works but like.. its pasted from amalgam and now im gonna have people saying "ohh ohh sgit aamalgam paste made by grizz" or some shit like that but theyve never made a selfcoded cheat
  so why can they talk?

  also rei had like the shit as the fake angles and im not sure if im supposed to do that so i changed some stuff but it maybe work maybe not work if u paste it

*/

void CAnimFix::Update(CUserCmd* pCmd) {
    const auto pLocal = H::Entities->GetLocal();
    const auto pWeapon = H::Entities->GetWeapon();

    if (!pLocal || pLocal->deadflag())
        return;

    auto pAnimState = pLocal->GetAnimState();
    if (!pAnimState)
        return;

    pCmd->viewangles.x = std::clamp(pCmd->viewangles.x, -89.f, 89.f);

    float flOldFrameTime = I::GlobalVars->frametime;
    int nOldSequence = pLocal->m_nSequence();
    float flOldCycle = pLocal->m_flCycle();
    auto& pOldPoseParams = pLocal->m_flPoseParameter();
    char pOldAnimState[sizeof(CTFPlayerAnimState)] = {};
    memcpy(pOldAnimState, pAnimState, sizeof(CTFPlayerAnimState));

    I::GlobalVars->frametime = 0.f;
    pAnimState->m_flCurrentFeetYaw = pCmd->viewangles.y;
    pAnimState->m_flEyeYaw = pCmd->viewangles.y;
    pAnimState->Update(pCmd->viewangles.y, pCmd->viewangles.x);

    int BonesSetup = pLocal->SetupBones(BoneMatrix, 128, BONE_USED_BY_ANYTHING, I::GlobalVars->curtime);

    I::GlobalVars->frametime = flOldFrameTime;
    pLocal->m_nSequence() = nOldSequence;
    pLocal->m_flCycle() = flOldCycle;
    pLocal->m_flPoseParameter() = pOldPoseParams;
    memcpy(pAnimState, pOldAnimState, sizeof(CTFPlayerAnimState));
}