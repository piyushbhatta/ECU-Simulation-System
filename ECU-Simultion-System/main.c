#include <stdio.h>
#include <stdint.h>
#include <x86intrin.h>
#include <inttypes.h>

#include "lib/types.h"
#include "lib/input.h"
#include "lib/mode.h"
#include "lib/control.h"
#include "lib/fault.h"
#include "lib/state.h"
#include "lib/log.h"
#include "lib/compare.h"

#define MAX_TESTS 50

static inline uint64_t rdtsc_start()
{
    _mm_lfence();
    return __rdtsc();
}

static inline uint64_t rdtsc_end()
{
    uint64_t t = __rdtsc();
    _mm_lfence();
    return t;
}

void init_perf(Perf *p)
{
    p->min = UINT64_MAX;
    p->max = 0;
    p->total = 0;
}

void update_perf(Perf *p, uint64_t cycles)
{
    if (cycles < p->min) p->min = cycles;
    if (cycles > p->max) p->max = cycles;
    p->total += cycles;
}

/* ===== PRINT TABLE ===== */
void print_table(Perf read, Perf val, Perf mode,
                 Perf ctrl, Perf fault, Perf state,
                 int subcycles, int test_num)
{
    uint64_t read_avg  = read.total / subcycles;
    uint64_t val_avg   = val.total / subcycles;
    uint64_t mode_avg  = mode.total / subcycles;
    uint64_t ctrl_avg  = ctrl.total / subcycles;
    uint64_t fault_avg = fault.total / subcycles;
    uint64_t state_avg = state.total / subcycles;

    printf("\n=========== TEST CASE %d PERFORMANCE ===========\n\n", test_num);

    printf("%-25s %-8s %-8s %-8s %-8s %-8s\n",
           "Module", "Min", "Max", "Avg", "Code", "Data");

    printf("--------------------------------------------------------------\n");

    printf("%-25s %-8lu %-8lu %-8lu %-8d %-8d\n",
           "read_inputs", read.min, read.max, read_avg, 354, 32);

    printf("%-25s %-8lu %-8lu %-8lu %-8d %-8d\n",
           "validate_inputs", val.min, val.max, val_avg, 260, 0);

    printf("%-25s %-8lu %-8lu %-8lu %-8d %-8d\n",
           "update_mode", mode.min, mode.max, mode_avg, 259, 0);

    printf("%-25s %-8lu %-8lu %-8lu %-8d %-8d\n",
           "run_control_checks", ctrl.min, ctrl.max, ctrl_avg, 368, 0);

    printf("%-25s %-8lu %-8lu %-8lu %-8d %-8d\n",
           "update_fault_status", fault.min, fault.max, fault_avg, 55, 0);

    printf("%-25s %-8lu %-8lu %-8lu %-8d %-8d\n",
           "evaluate_state", state.min, state.max, state_avg, 182, 0);

    printf("\n=============================================================\n");
}

int main()
{
    freopen("system/output.txt", "w", stdout);

    init_input();   // open + load file once

    VehicleInput in;
    VehicleStatus st = {MODE_OFF, MODE_OFF, STATE_NORMAL, 0};
    FaultStatus f = {0};

    VehicleMode actual_modes[MAX_TESTS];
    SystemState actual_states[MAX_TESTS];

    int test_count = 0;

    while (1)
    {
        int subcycle = 0;
        int printed = 0;

        Perf read_p, val_p, mode_p, ctrl_p, fault_p, state_p;

        init_perf(&read_p);
        init_perf(&val_p);
        init_perf(&mode_p);
        init_perf(&ctrl_p);
        init_perf(&fault_p);
        init_perf(&state_p);

        int tc_subcycles = 0;

        st.accSeen = 0;

        while (1)
        {
            uint64_t start, end;

            start = rdtsc_start();
            int status = read_inputs(&in);
            end = rdtsc_end();
            update_perf(&read_p, end - start);

            if (status == -1)
                goto END_PROGRAM;

            if (status == 0)
            {
                if (subcycle == 0) continue;
                else break;
            }

            if (!printed)
            {
                printf("\n==== TEST CASE %d ====\n", test_count + 1);
                printed = 1;
            }

            printf("\n---- SUBCYCLE %c ----\n", 'A' + subcycle);

            start = rdtsc_start();
            validate_inputs(&in, &f);
            end = rdtsc_end();
            update_perf(&val_p, end - start);

            f.errorCount = 0;

            start = rdtsc_start();
            update_fault_status(&f);
            end = rdtsc_end();
            update_perf(&fault_p, end - start);

            start = rdtsc_start();
            run_control_checks(&in, &st, &f);
            end = rdtsc_end();
            update_perf(&ctrl_p, end - start);

            /* MODE */
            start = rdtsc_start();
            update_mode(&st, &in, &f, subcycle);
            end = rdtsc_end();
            update_perf(&mode_p, end - start);

            /* STATE */
            start = rdtsc_start();
            evaluate_state(&st, &f);
            end = rdtsc_end();
            update_perf(&state_p, end - start);

            tc_subcycles++;

            print_summary(&in, &st, &f, 0);

            printf("\n------------------------------\n");

            subcycle++;
        }

        if (subcycle > 0)
        {
            actual_modes[test_count] = st.currentMode;
            actual_states[test_count] = st.systemState;

            print_table(read_p, val_p, mode_p, ctrl_p, fault_p, state_p,
                        tc_subcycles, test_count + 1);

            printf("\n==============================\n");

            test_count++;

            st.currentMode = MODE_OFF;
            st.systemState = STATE_NORMAL;

            f.overtempCount = 0;
            f.overspeedCount = 0;
            f.invalidGearCount = 0;
            f.invalidModeCount = 0;
            f.safeCycleCount = 0;
        }
    }

END_PROGRAM:

    fflush(stdout);

    compare_results(actual_modes, actual_states);

    return 0;
}