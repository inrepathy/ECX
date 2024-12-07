#include "../../SDK/SDK.h"

#include "../Features/Menu/Menu.h"
#include "../Features/ESP/ESP.h"
#include "../Features/Radar/Radar.h"
#include "../Features/MiscVisuals/MiscVisuals.h"
#include "../Features/SpectatorList/SpectatorList.h"
#include "../../App/Features/Crits/Crits.h"
#include "../Features/SpyCamera/SpyCamera.h"
#include "../Features/SpyWarning/SpyWarning.h"
#include "../Features/TeamWellBeing/TeamWellBeing.h"
#include "../Features/SeedPred/SeedPred.h"
#include "../Features/BindsList/BindsList.h"

MAKE_HOOK(IEngineVGuiInternal_Paint, Memory::GetVFunc(I::EngineVGui, 14), void, __fastcall,
	void *ecx, int mode)
{
	CALL_ORIGINAL(ecx, mode);

	if (mode & PAINT_UIPANELS)
	{
		gDraw().UpdateW2SMatrix();

		I::MatSystemSurface->StartDrawing();
		{
			F::ESP->Run();
			F::TeamWellBeing->Run();
			F::MiscVisuals->ShiftBar();
			F::Radar->Run();
			F::SpectatorList->Run();
			F::Crits->Indicator();
			F::BindsList->Run();
			F::MiscVisuals->TrailVisualizer();
			F::MiscVisuals->AAVisualizer();
			F::MiscVisuals->AspectRatio();
			F::MiscVisuals->AimbotFOVCircle();
			F::SpyCamera->Run();
			F::SpyWarning->Run();
			F::SeedPred->Paint();
			F::Menu->Run();
		}
		I::MatSystemSurface->FinishDrawing();
	}
}
