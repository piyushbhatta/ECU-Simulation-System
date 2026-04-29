#include "fault.h"

void update_fault_status(FaultStatus *f)
{
    if (f->overtempCount > 0)
        f->persistentOvertemp++;
    else
        f->persistentOvertemp = 0;
}

void reset_cycle_faults(FaultStatus *f)
{
    f->overtempCount = 0;
    f->overspeedCount = 0;
    f->invalidGearCount = 0;
    f->invalidModeCount = 0;
}