#ifndef INPUT_H
#define INPUT_H

#include "types.h"
void init_input();
int read_inputs(VehicleInput *in);
void validate_inputs(VehicleInput *in, FaultStatus *f);

#endif