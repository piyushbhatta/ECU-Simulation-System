#include "mode.h"
#include "error.h"

void update_mode(VehicleStatus *st, VehicleInput *in, FaultStatus *f, int subcycle)
{
    st->previousMode = st->currentMode;

    if (in->requestedMode == MODE_ACC)
    {
        st->accSeen = 1;
    }

    if (in->requestedMode == MODE_FAULT)
    {
        st->currentMode = MODE_FAULT;
        return;
    }

    if (st->previousMode == MODE_FAULT)
    {
        st->currentMode = MODE_FAULT;
        return;
    }

    if (f->persistentOvertemp >= 3)
    {
        st->currentMode = MODE_FAULT;
        return;
    }

    if (subcycle > 0 &&
        st->previousMode == MODE_OFF &&
        in->requestedMode == MODE_IGNITION_ON &&
        st->accSeen == 0)
    {   
        f->majorFaultCount++;
        f->invalidModeCount++;
        add_error(f, ERR_ILLEGAL_TRANSITION);

        st->currentMode = MODE_FAULT;
        return;
    }

    st->currentMode = in->requestedMode;
}