#include "../../SDK/SDK.h"

#include "../Features/CFG.h"
#include "../Features/SeedPred/SeedPred.h"

MAKE_SIGNATURE(FX_FireBullets, "client.dll", "48 89 5C 24 ? 48 89 74 24 ? 4C 89 4C 24 ? 55", 0x0);

MAKE_HOOK(FX_FireBullets, Signatures::FX_FireBullets.Get(), void, __cdecl,
	void* pWpn, int iPlayer, const Vector& vecOrigin, const QAngle& vecAngles, int iWeapon, int iMode, int iSeed, float flSpread, float flDamage, bool bCritical)
{
	if (CFG::Exploits_SeedPred_Active)
	{
		iSeed = F::SeedPred->GetSeed();
	}

	CALL_ORIGINAL(pWpn, iPlayer, vecOrigin, vecAngles, iWeapon, iMode, iSeed, flSpread, flDamage, bCritical);
}
