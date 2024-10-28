#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(CEconItemSchema_GetItemDefinition, "client.dll", "89 54 24 ? 53 48 83 EC ? 48 8B D9 48 8D 54 24 ? 48 81 C1 ? ? ? ? E8 ? ? ? ? 85 C0", 0x0);

struct CEconItemDefinition_t
{
	byte pad0[524];
	uint32 m_unEquipRegionMask;
	uint32 m_unEquipRegionConflictMask;
};

MAKE_HOOK(CEconItemSchema_GetItemDefinition, Signatures::CEconItemSchema_GetItemDefinition.Get(), CEconItemDefinition_t*, __fastcall,
    void* ecx, int iItemIndex)
{
	if (CFG::Misc_Equip_Region_Unlock)
	{
		const auto item = CALL_ORIGINAL(ecx, iItemIndex);

		if (item)
		{
			item->m_unEquipRegionMask = 0;
			item->m_unEquipRegionConflictMask = 0;
		}

		return item;
	}

	return CALL_ORIGINAL(ecx, iItemIndex);
}
