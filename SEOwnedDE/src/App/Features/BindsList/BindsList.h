#pragma once

#include "../../../SDK/SDK.h"

class CBindsList
{
	struct CBindsList_t
	{
		std::wstring Name = {};
		int m_nMode = 0;
	};

	std::vector<CBindsList> m_vecSpectators = {};

	bool GetSpectators();
	void Drag();

public:
	void Run();
};

MAKE_SINGLETON_SCOPED(CBindsList, BindsList, F);
