#include "../../SDK/SDK.h"

#include "../Features/CFG.h"
#include "../Features/SeedPred/SeedPred.h"
#include "../Features/Resolver/Resolver.h"

MAKE_SIGNATURE(FX_FireBullets, "client.dll", "48 89 5C 24 ? 48 89 74 24 ? 4C 89 4C 24 ? 55", 0x0);

MAKE_HOOK(FX_FireBullets, Signatures::FX_FireBullets.Get(), void, __cdecl,
	void* pWpn, int iPlayer, const Vector& vecOrigin, const QAngle& vecAngles,
	int iWeapon, int iMode, int iSeed, float flSpread, float flDamage, bool bCritical,
	CClientModeShared* ecx, float flInputSampleTime, CUserCmd* pCmd) {

	C_TFPlayer* pPlayer = reinterpret_cast<C_TFPlayer*>(I::ClientEntityList->GetClientEntity(iPlayer)); // this is bad. :frown:


	if (CFG::Exploits_SeedPred_Active)
	{
		iSeed = F::SeedPred->GetSeed();
	}

	int iIndex = 0;
	Vec3 vAngles;

	if (CFG::Resolver) // TODO: make this change if you only have clamp on and so forth.. but like i have it already in OnShot so like whats the point??? maybe im  jsut retarded
	{
		F::Resolver->OnShot(pCmd, pPlayer, iIndex, vAngles);
	}

	CALL_ORIGINAL(pWpn, iPlayer, vecOrigin, vecAngles, iWeapon, iMode, iSeed, flSpread, flDamage, bCritical, ecx, flInputSampleTime, pCmd);
}
