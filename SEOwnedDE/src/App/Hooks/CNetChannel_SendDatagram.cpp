#include "../../SDK/SDK.h"
#include "../Features/LagRecords/LagRecords.h"

MAKE_SIGNATURE(CNetChannel_SendDatagram, "engine.dll", "40 55 57 41 56 48 8D AC 24", 0x0);

namespace S {
    int CNetChannel_SendDatagram;
}

// Make the hook
MAKE_HOOK(CNetChannel_SendDatagram, S::CNetChannel_SendDatagram, int, __cdecl,
    CNetChannel* pNetChan, bf_write* datagram)
{
    if (datagram)
        return CALL_ORIGINAL(pNetChan, datagram);

    F::LagRecordMatrixHelper->AdjustPing(pNetChan);
    const int iReturn = CALL_ORIGINAL(pNetChan, datagram);
    F::LagRecordMatrixHelper->RestorePing(pNetChan);
    return iReturn;
}
