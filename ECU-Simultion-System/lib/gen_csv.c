#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE *in = fopen("perf/size_raw.txt", "r");   
    FILE *out = fopen("system/module_size.csv", "w");

    if (!in || !out)
    {
        perror("File error");   // better debug
        return 1;
    }
    char line[256];
    char func[100];
    unsigned long size;

    fprintf(out, "Function,Code Size (bytes),Data Size (bytes)\n");

    while (fgets(line, sizeof(line), in))
    {
        if (sscanf(line, "%*x %lx T %s", &size, func) == 2)
        {
            if (strcmp(func, "read_inputs") == 0 ||
                strcmp(func, "validate_inputs") == 0 ||
                strcmp(func, "run_control_checks") == 0 ||
                strcmp(func, "update_mode") == 0 ||
                strcmp(func, "evaluate_state") == 0 ||
                strcmp(func, "update_fault_status") == 0)   // ✅ added
            {
                int dataSize = 0;

                if (strcmp(func, "read_inputs") == 0)
                    dataSize = 32;

                fprintf(out, "%s,%lu,%d\n", func, size, dataSize);
            }
        }
    }

    fclose(in);
    fclose(out);

    printf("CSV generated: module_size.csv\n");

    return 0;
}