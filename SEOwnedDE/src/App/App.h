#pragma once

#include "../SDK/SDK.h"

class CApp
{
	bool bUnload = false;

public:
	void Start();
	void Loop();
	void Shutdown();
};

MAKE_SINGLETON(CApp, App);