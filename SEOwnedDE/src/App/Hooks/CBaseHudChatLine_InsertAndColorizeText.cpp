#include "../../SDK/SDK.h"

#include "../Features/CFG.h"
#include "../Features/Players/Players.h"

MAKE_SIGNATURE(CBaseHudChatLine_InsertAndColorizeText, "client.dll", "44 89 44 24 ? 55 53 56 57", 0x0);

MAKE_HOOK(CBaseHudChatLine_InsertAndColorizeText, Signatures::CBaseHudChatLine_InsertAndColorizeText.Get(), void, __fastcall,
	void* ecx, wchar_t* buf, int clientIndex)
{
	if (CFG::Visuals_Chat_Name_Tags && ecx)
	{
		std::wstring str{ buf };
		std::wstring prefix{};

		if (clientIndex == I::EngineClient->GetLocalPlayer())
		{
			prefix = std::format(L"\x8{}[Local] \x3", CFG::Color_Local.toHexStrW());
		}

		else
		{
			if (C_TFPlayer* pl{ I::ClientEntityList->GetClientEntity(clientIndex)->As<C_TFPlayer>() })
			{
				if (pl->IsPlayerOnSteamFriendsList())
				{
					prefix = std::format(L"\x8{}[Friend] \x3", CFG::Color_Friend.toHexStrW());
				}
			}

			PlayerPriority pi{};
			if (F::Players->GetInfo(clientIndex, pi))
			{
				if (pi.Ignored)
				{
					prefix = std::format(L"\x8{}[Ignored] \x3", CFG::Color_Friend.toHexStrW());
				}

				if (pi.Cheater)
				{
					prefix = std::format(L"\x8{}[Cheater] \x3", CFG::Color_Cheater.toHexStrW());
				}

				if (pi.RetardLegit)
				{
					prefix = std::format(L"\x8{}[Retard Legit] \x3", CFG::Color_RetardLegit.toHexStrW());
				}
			}
		}

		if (!prefix.empty())
		{
			const int nameStart = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(ecx) + 928);
			const int nameLength = *reinterpret_cast<int*>(reinterpret_cast<uintptr_t>(ecx) + 864);

			str.insert(nameStart + nameLength, L"\x1");
			str.insert(0, prefix);

			CALL_ORIGINAL(ecx, const_cast<wchar_t*>(str.c_str()), clientIndex);

			return;
		}
	}

	CALL_ORIGINAL(ecx, buf, clientIndex);
}
