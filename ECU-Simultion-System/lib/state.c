#include "state.h"
#include "error.h"

const char* get_state_name(SystemState s)
{
    switch (s)
    {
        case STATE_NORMAL: return "NORMAL";
        case STATE_DEGRADED: return "DEGRADED";
        case STATE_SAFE: return "SAFE";
        default: return "UNKNOWN";
    }
}

int evaluate_state(VehicleStatus *st, FaultStatus *f)
{
    SystemState prev = st->systemState;

    if (f->overtempCount > 0)
        f->persistentOvertemp++;
    else
        f->persistentOvertemp = 0;
    
    if (f->isCritical)
    {
        st->systemState = STATE_SAFE;
    }
    else if (f->majorFaultCount >= 2 || f->persistentOvertemp >= 3)
    {
        st->systemState = STATE_SAFE;
    }
    else if (f->majorFaultCount == 1 || f->persistentHighTemp > 1)
    {
        st->systemState = STATE_DEGRADED;
    }
    else
    {
        st->systemState = STATE_NORMAL;
    }

    return 0;
}