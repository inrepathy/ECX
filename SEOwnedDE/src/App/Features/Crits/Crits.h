#pragma once

#include "../../../SDK/SDK.h"

class CCrits
{
public:
	void Run(CUserCmd* pCmd);
	void Indicator();
};

MAKE_SINGLETON_SCOPED(CCrits, Crits, F);
