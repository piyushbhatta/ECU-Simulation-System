#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "input.h"
#include "error.h"

static FILE *fp = NULL;
static char buffer[1000][100];
static int total_lines = 0;
static int idx = 0;
static VehicleInput lastValid = {0, 0, 0, MODE_OFF};
void init_input()
{
    fp = fopen("system/input.txt", "r");
    if (!fp) exit(1);

    while (fgets(buffer[total_lines], 100, fp))
        total_lines++;

    fclose(fp);
}


static inline int fast_atoi(char **p)
{
    int val = 0;

    while (**p == ' ') (*p)++;

    while (**p >= '0' && **p <= '9')
    {
        val = val * 10 + (**p - '0');
        (*p)++;
    }

    return val;
}

int read_inputs(VehicleInput *in)
{
    while (idx < total_lines)
    {
        char *p = buffer[idx++];

        if (*p <= ' ')
            continue;

        if (p[0] == 'E')
            return 0;

        // Manual parsing (FASTEST)
        in->speed = fast_atoi(&p);
        in->temperature = fast_atoi(&p);
        in->gear = fast_atoi(&p);
        in->requestedMode = (VehicleMode)fast_atoi(&p);

        return 1;
    }

    return -1;
}


void validate_inputs(VehicleInput *in, FaultStatus *f)
{
    int s = in->speed;
    int t = in->temperature;
    int g = in->gear;
    int m = in->requestedMode;

    int valid = 1;

    if ((unsigned)s > 200)
    {
        in->speed = lastValid.speed;
        valid = 0;
        add_error(f, ERR_INVALID_MODE);
    }

    if (t < -40 || t > 150)
    {
        in->temperature = lastValid.temperature;
        valid = 0;
        add_error(f, ERR_INVALID_MODE);
    }

    if ((unsigned)g > 5)
    {
        add_error(f, ERR_INVALID_GEAR);
    }

    if ((unsigned)m > MODE_FAULT)
    {
        in->requestedMode = lastValid.requestedMode;
        valid = 0;
        add_error(f, ERR_INVALID_MODE);
    }

    if (valid)
        lastValid = *in;
}