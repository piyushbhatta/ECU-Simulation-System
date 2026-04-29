#ifndef STATE_H
#define STATE_H

#include "types.h"

int evaluate_state(VehicleStatus *st, FaultStatus *f);
const char* get_state_name(SystemState s);

#endif