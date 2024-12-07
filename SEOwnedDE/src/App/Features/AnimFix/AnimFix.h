#pragma once

#include "../../SEOwnedDE/src/SDK/SDK.h"

class CAnimFix
{
public:
	void Update(CUserCmd* pCmd);
	int TicksElapsed = 0;
};

MAKE_SINGLETON_SCOPED(CAnimFix, AnimFix, F)