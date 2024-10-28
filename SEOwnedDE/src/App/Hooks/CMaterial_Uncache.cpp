#include "../../SDK/SDK.h"

#include "../Features/Materials/Materials.h"
#include "../Features/Outlines/Outlines.h"

MAKE_SIGNATURE(CMaterial_Uncache, "materialsystem.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B F9", 0x0);

/*
MAKE_HOOK(CMaterial_Uncache, Signatures::CMaterial_Uncache.Get(), void, __fastcall,
	void* ecx, bool bPreserveVars)
{
	if (auto pMaterial = reinterpret_cast<IMaterial *>(reinterpret_cast<DWORD>(ecx) + 0x90))
	{
		if (F::Materials->IsUsedMaterial(pMaterial) && !F::Materials->IsCleaningUp())
			return;

		if (F::Outlines->IsUsedMaterial(pMaterial) && !F::Outlines->IsCleaningUp())
			return;
	}

	CALL_ORIGINAL(ecx, bPreserveVars);
}
*/