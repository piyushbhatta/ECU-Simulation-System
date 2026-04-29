#include <stdio.h>
#include "log.h"
#include "state.h"
#include "error.h"

const char* get_mode_name(VehicleMode m)
{
    switch (m)
    {
        case MODE_OFF: return "OFF";
        case MODE_ACC: return "ACC";
        case MODE_IGNITION_ON: return "IGNITION_ON";
        case MODE_FAULT: return "FAULT";
        default: return "UNKNOWN";
    }
}

void print_summary(VehicleInput *in, VehicleStatus *st, FaultStatus *f, int cycle)
{
    int speed = in->speed;
    int temp = in->temperature;
    int gear = in->gear;

    const char* modeStr = get_mode_name(st->currentMode);
    const char* stateStr = get_state_name(st->systemState);

    printf(
        "\nSpeed = %d km/h\n"
        "Temperature = %d C\n"
        "Gear = %d\n"
        "Mode = %s\n"
        "\nFault Counters:\n"
        "Overheat = %d\n"
        "Invalid Gear = %d\n"
        "Overspeed = %d\n"
        "\nSystem State: %s\n",
        speed,
        temp,
        gear,
        modeStr,
        f->overtempCount,
        f->invalidGearCount,
        f->overspeedCount,
        stateStr
    );

    log_errors(f, "SYSTEM");
}