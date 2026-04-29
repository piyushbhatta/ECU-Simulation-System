#ifndef TYPES_H
#include <stdint.h>
#define TYPES_H

typedef enum
{
    MODE_OFF = 0,
    MODE_ACC,
    MODE_IGNITION_ON,
    MODE_FAULT
} VehicleMode;

typedef enum
{
    STATE_NORMAL = 0,
    STATE_DEGRADED,
    STATE_SAFE
} SystemState;

typedef enum
{
    ERR_NONE = 0,

    ERR_OVERTEMP,
    ERR_OVERSPEED,
    ERR_INVALID_GEAR,
    ERR_INVALID_MODE,
    ERR_ILLEGAL_TRANSITION,

    WARN_HIGH_TEMP,
    WARN_OVERSPEED
} ErrorCode;

typedef struct {
    uint64_t min;
    uint64_t max;
    uint64_t total;
} Perf;

typedef struct
{
    int speed;
    int temperature;
    int gear;
    VehicleMode requestedMode;
} VehicleInput;

typedef struct
{   
    VehicleMode currentMode;
    VehicleMode previousMode;
    SystemState systemState;
    int accSeen;
} VehicleStatus;

typedef struct
{
    int overtempCount;
    int overspeedCount;
    int invalidGearCount;
    int invalidModeCount;

    int persistentOvertemp;
    int persistentHighTemp;

    int safeCycleCount;

    ErrorCode errors[5];
    int errorCount;

    int isCritical;
    int majorFaultCount;

} FaultStatus;

typedef int Std_ReturnType;

#define E_OK 0
#define E_NOT_OK 1

#endif