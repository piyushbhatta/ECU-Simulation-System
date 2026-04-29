#include "control.h"
#include "error.h"

void run_control_checks(VehicleInput *in, VehicleStatus *st, FaultStatus *f)
{
    int temp = in->temperature;
    int gear = in->gear;
    int speed = in->speed;

    int faultFound = 0;
    int warningFound = 0;

    f->isCritical = 0;
    f->majorFaultCount = 0;

    if (temp > 110)
    {
        f->overtempCount++;
        f->isCritical = 1;
        faultFound = 1;

        st->currentMode = MODE_FAULT;
        add_error(f, ERR_OVERTEMP);
    }

    if (gear < 0 || gear > 5)
    {
        f->invalidGearCount++;
        f->majorFaultCount++;
        faultFound = 1;

        add_error(f, ERR_INVALID_GEAR);
    }

    if (speed > 120)
    {
        f->overspeedCount++;
        f->majorFaultCount++;
        faultFound = 1;

        add_error(f, ERR_OVERSPEED);
    }

    if (temp > 95 && temp <= 110)
    {
        f->majorFaultCount++;
        warningFound = 1;
        f->persistentHighTemp=1;
        add_error(f, WARN_HIGH_TEMP);
    }

    if (!faultFound && !warningFound)
    {
        f->persistentHighTemp = 0;
    }
}   