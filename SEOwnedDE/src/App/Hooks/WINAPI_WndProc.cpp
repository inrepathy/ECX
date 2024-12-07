#include "WINAPI_WndProc.h"

#include "../Features/Menu/Menu.h"

LRESULT __stdcall Hooks::WINAPI_WndProc::Func(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (F::Menu->IsOpen() && H::Input->IsGameFocused())
	{
		if (F::Menu->m_bWantTextInput || F::Menu->m_bInKeybind)
		{
			I::InputSystem->ResetInputState();
			return 1;
		}

		if (uMsg >= WM_MOUSEFIRST && WM_MOUSELAST >= uMsg)
			return 1;
	}

	return CallWindowProc(Original, hWnd, uMsg, wParam, lParam);
}

void Hooks::WINAPI_WndProc::Init()
{
	hwWindow = SDKUtils::GetTeamFortressWindow();
	Original = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hwWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Func)));
}

void Hooks::WINAPI_WndProc::Release()
{
	SetWindowLongPtr(hwWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Original));
}
