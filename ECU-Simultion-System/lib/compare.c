#include <stdio.h>
#include <string.h>
#include "compare.h"

VehicleMode get_mode(const char *str)
{
    if (strcmp(str, "OFF") == 0) return MODE_OFF;
    if (strcmp(str, "ACC") == 0) return MODE_ACC;
    if (strcmp(str, "IGNITION_ON") == 0) return MODE_IGNITION_ON;
    if (strcmp(str, "FAULT") == 0) return MODE_FAULT;
    return MODE_OFF;
}

SystemState get_state(const char *str)
{
    if (strcmp(str, "NORMAL") == 0) return STATE_NORMAL;
    if (strcmp(str, "DEGRADED") == 0) return STATE_DEGRADED;
    if (strcmp(str, "SAFE") == 0) return STATE_SAFE;
    return STATE_NORMAL;
}

void compare_results(VehicleMode *actual_modes,
                     SystemState *actual_states)
{
    FILE *fp = fopen("system/expected.json", "r");
    FILE *out = fopen("system/status.txt", "w");

    if (!fp || !out)
    {
        printf("File error\n");
        return;
    }

    char line[256];
    char mode[50], state[50];

    int i = 0;

    while (fgets(line, sizeof(line), fp))
    {
        char *m_ptr = strstr(line, "\"mode\"");
        char *s_ptr = strstr(line, "\"state\"");

        if (m_ptr && s_ptr)
        {
            sscanf(m_ptr, "\"mode\": \"%[^\"]\"", mode);
            sscanf(s_ptr, "\"state\": \"%[^\"]\"", state);

            if (actual_modes[i] == get_mode(mode) &&
                actual_states[i] == get_state(state))
            {
                fprintf(out, "Test %d: PASS\n", i + 1);
            }
            else
            {
                fprintf(out, "Test %d: FAIL\n", i + 1);
            }

            i++;
        }
    }

    if (i == 0)
    {
        fprintf(out, "No test cases compared\n");
    }

    fclose(fp);
    fclose(out);
}