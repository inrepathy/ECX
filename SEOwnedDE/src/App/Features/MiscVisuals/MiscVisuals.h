#pragma once

#include "../../../SDK/SDK.h"

class CMiscVisuals
{
public:
	void AimbotFOVCircle();
	void ViewModelSway();
	void DetailProps();
	void ShiftBar();
	void AspectRatio();

	void SniperLines();
	void ProjectileArc();
	void AAVisualizer();

	void TrailVisualizer();

	void CustomFOV(CViewSetup* pSetup);
	void Thirdperson(CViewSetup* pSetup);
};

MAKE_SINGLETON_SCOPED(CMiscVisuals, MiscVisuals, F);
