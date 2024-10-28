#include "../../SDK/SDK.h"

#include "../Features/CFG.h"

MAKE_SIGNATURE(R_DrawSkyBox, "engine.dll", "48 8B C4 55 53 41 54 41 55", 0x0);

MAKE_HOOK(R_DrawSkyBox, Signatures::R_DrawSkyBox.Get(), void, __fastcall,
	float zFar, int nDrawFlags)
{
	if (CFG::Misc_Clean_Screenshot && I::EngineClient->IsTakingScreenshot())
	{
		CALL_ORIGINAL(zFar, nDrawFlags);

		return;
	}

	const auto& custom = CFG::Visuals_Custom_Skybox_Texture_Name;

	if (!custom.empty())
	{
		static auto sv_skyname{ I::CVar->FindVar("sv_skyname") };
		const std::string original = sv_skyname->GetString();

		sv_skyname->SetValue(custom.c_str());

		CALL_ORIGINAL(zFar, nDrawFlags);

		sv_skyname->SetValue(original.c_str());

		return;
	}

	CALL_ORIGINAL(zFar, nDrawFlags);
}

/*
MAKE_SIGNATURE(R_LoadSkys, "engine.dll", "40 53 48 81 EC ? ? ? ? 8B 05 ? ? ? ? B3", 0x0);

MAKE_HOOK(R_LoadSkys, Signatures::R_LoadSkys.Get(), void, __cdecl
	)
{
	if (CFG::Misc_Clean_Screenshot && I::EngineClient->IsTakingScreenshot())
	{
		CALL_ORIGINAL();

		return;
	}

	const auto& custom = CFG::Visuals_Custom_Skybox_Texture_Name;

	if (!custom.empty())
	{
		static auto sv_skyname{ I::CVar->FindVar("sv_skyname") };
		const std::string original = sv_skyname->GetString();

		sv_skyname->SetValue(custom.c_str());

		CALL_ORIGINAL();

		sv_skyname->SetValue(original.c_str());

		return;
	}

	CALL_ORIGINAL();
}
*/