#include "c_tf_player.h"

#include "../../App/Features/Players/Players.h"

bool C_TFPlayer::IsPlayerOnSteamFriendsList()
{
	auto result{ reinterpret_cast<bool(__fastcall *)(void *, void *)>(Signatures::CTFPlayer_IsPlayerOnSteamFriendsList.Get())(this, this) };

	if (!result)
	{
		PlayerPriority info{};

		return F::Players->GetInfo(entindex(), info) && info.Ignored;
	}

	return result;
}