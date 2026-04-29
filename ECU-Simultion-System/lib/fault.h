#ifndef FAULT_H
#define FAULT_H

#include "types.h"

void update_fault_status(FaultStatus *f);
void reset_cycle_faults(FaultStatus *f);

#endif