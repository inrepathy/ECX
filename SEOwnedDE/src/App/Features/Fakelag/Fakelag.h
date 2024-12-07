#pragma once
#define FAKELAG
#ifdef FAKELAG // Check if FAKELAG is defined
class CFakelag {
public:
    void Run(CUserCmd* pCmd, bool* pSendPacket);
    int MAX_TICKS = 22;
    Vec3 vLastPosition{};
};

MAKE_SINGLETON_SCOPED(CFakelag, Fakelag, F)

#endif // End of FAKELAG conditional
