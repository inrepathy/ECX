#pragma once
#include "imaterialsystem.h"

MAKE_SIGNATURE(RenderLine, "engine.dll", "48 89 5C 24 ? 48 89 74 24 ? 44 89 44 24", 0x0);
MAKE_SIGNATURE(RenderBox, "engine.dll", "48 83 EC ? 8B 84 24 ? ? ? ? 4D 8B D8", 0x0);
MAKE_SIGNATURE(RenderWireframeBox, "engine.dll", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 49 8B F9", 0x0);

namespace RenderUtils
{
	inline void RenderLine(const Vector& v1, const Vector& v2, Color_t c, bool bZBuffer)
	{
		using Fn = void(__cdecl *)(const Vector&, const Vector&, Color_t, bool);
		reinterpret_cast<Fn>(Signatures::RenderLine.Get())(v1, v2, c, bZBuffer);
	}

	inline void RenderBox(const Vector& origin, const QAngle& angles, const Vector& mins, const Vector& maxs, Color_t c, bool bZBuffer, bool bInsideOut = false)
	{
		using Fn = void(__cdecl *)(const Vector&, const QAngle&, const Vector&, const Vector&, Color_t, bool, bool);
		reinterpret_cast<Fn>(Signatures::RenderBox.Get())(origin, angles, mins, maxs, c, bZBuffer, bInsideOut);
	}

	inline void RenderWireframeBox(const Vector& vOrigin, const QAngle& angles, const Vector& vMins, const Vector& vMaxs, Color_t c, bool bZBuffer)
	{
		using Fn = void(__cdecl *)(const Vector&, const QAngle&, const Vector&, const Vector&, Color_t, bool);
		reinterpret_cast<Fn>(Signatures::RenderWireframeBox.Get())(vOrigin, angles, vMins, vMaxs, c, bZBuffer);
	}
}
