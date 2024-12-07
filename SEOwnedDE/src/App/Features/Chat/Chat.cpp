#include "../../../SDK/SDK.h"

#include "../CFG.h"

MAKE_SIGNATURE(CTFGameRules_ModifySentChat, "client.dll", "48 89 5C 24 ? 57 48 83 EC ? 80 B9 ? ? ? ? ? 41 8B F8", 0x0);

//pasted from mfed :muscle:
const std::vector<std::pair<std::string, std::string>> patterns
{
    { "r", "w" },
    { "l", "w" },
    { "R", "W" },
    { "L", "W" },
    { "n([aeiou])", "ny$1" },
    { "N([aeiou])", "Ny$1" },
    { "N([AEIOU])", "NY$1" }
};

const std::vector<std::string> suffixes
{
    " UwU", " OwO", " owo", " uwu", " nwn", " :3", " >w<", " ^w^", " <3", " uvu", " TvT", " rwar", " :P", " x3",
    " ^_^", " :3c", " -w-", " :D", " :)", " <.<", " >.>", " -.-", " o.o", " :o", " :T", " :|"
}; 

std::string owoify(std::string text)
{
    for (auto &pattern : patterns)
    {
        std::regex reg(pattern.first);

        text = std::regex_replace(text, reg, pattern.second);
    }

    if (text.size() < 124)
    {
        int suffix{ I::UniformRandomStream->RandomInt(0, static_cast<int>(suffixes.size() - 1)) };

        text += suffixes[suffix];
    }

    return text;
}

MAKE_HOOK(IVEngineClient013_ClientCmd_Unrestricted, Memory::GetVFunc(I::EngineClient, 106), void, __fastcall,
    void* ecx, const char *szCmdString)
{
    if (CFG::Misc_Chat_Owoify)
    {
        if (strstr(szCmdString, "say"))
        {
            std::string cmdString(szCmdString);

            if (cmdString.rfind("say", 0) != 0)
            {
                return CALL_ORIGINAL(ecx, szCmdString);
            }

            std::smatch match{};
            std::regex_search(cmdString, match, std::regex("(say.* )\"(.*)\""));

            cmdString = match[1].str() + "\"" + owoify(match[2].str()) + "\"";

            return CALL_ORIGINAL(ecx, cmdString.c_str());
        }
    }

    return CALL_ORIGINAL(ecx, szCmdString);
}

MAKE_HOOK(CTFGameRules_ModifySentChat, Signatures::CTFGameRules_ModifySentChat.Get(), void, __fastcall,
    void* ecx, char *pBuf, int iBufSize)
{
    static ConVar *tf_medieval_autorp{ I::CVar->FindVar("tf_medieval_autorp") };
    static ConVar *english{ I::CVar->FindVar("english") };

    if (CFG::Misc_Chat_Medieval && pBuf && iBufSize && tf_medieval_autorp && english)
    {
        struct s_CTFGameRules
        {
            char pad[1179]{};
            bool m_bPlayingMedieval{};
        };

        s_CTFGameRules *gamerules{ reinterpret_cast<s_CTFGameRules *>(ecx) };

        if (!tf_medieval_autorp || !english || !gamerules)
        {
            return CALL_ORIGINAL(ecx, pBuf, iBufSize);
        }

        bool originalEnglish{ english->GetBool() };
        bool originalAutoRP{ tf_medieval_autorp->GetBool() };
        bool originalPlaying{ gamerules->m_bPlayingMedieval };

        gamerules->m_bPlayingMedieval = true;
        tf_medieval_autorp->SetValue(true);
        english->SetValue(true);

        CALL_ORIGINAL(ecx, pBuf, iBufSize);

        gamerules->m_bPlayingMedieval = originalPlaying;
        tf_medieval_autorp->SetValue(originalAutoRP);
        english->SetValue(originalEnglish);

        return;
    }

    CALL_ORIGINAL(ecx, pBuf, iBufSize);
}
