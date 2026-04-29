#ifndef ERROR_H
#define ERROR_H

#include "types.h"

void add_error(FaultStatus *f, ErrorCode code);
void log_errors(FaultStatus *f, const char *module);

#endif