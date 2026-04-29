#include <stdio.h>
#include "error.h"

void add_error(FaultStatus *f, ErrorCode code)
{
    for (int i = 0; i < f->errorCount; i++)
    {
        if (f->errors[i] == code)
            return;
    }

    if (f->errorCount < 5)
    {
        f->errors[f->errorCount++] = code;
    }
}

void log_errors(FaultStatus *f, const char *module)
{
    for (int i = 0; i < f->errorCount; i++)
    {
        ErrorCode code = f->errors[i];

        if (code == WARN_HIGH_TEMP || code == WARN_OVERSPEED)
            printf("WARNING (%s): ", module);
        else
            printf("ERROR (%s): ", module);

        switch (code)
        {
            case ERR_OVERTEMP:
                printf("Overheat\n");
                break;

            case ERR_OVERSPEED:
                printf("Overspeed\n");
                break;

            case ERR_INVALID_GEAR:
                printf("Invalid Gear\n");
                break;

            case ERR_INVALID_MODE:
                printf("Invalid Mode\n");
                break;

            case ERR_ILLEGAL_TRANSITION:
                printf("Illegal Mode Transition\n");
                break;

            case WARN_HIGH_TEMP:
                printf("High Temperature\n");
                break;

            case WARN_OVERSPEED:
                printf("High Speed Warning\n");
                break;

            default:
                printf("Unknown\n");
        }
    }
}