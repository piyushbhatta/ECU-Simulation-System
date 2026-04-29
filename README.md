# Vehicle ECU Simulation (Embedded C Project)

---

## Overview

This project simulates a simplified Electronic Control Unit (ECU) using Embedded C concepts.
The system runs in a cyclic loop similar to real automotive ECUs.

It performs:
- Input reading & validation
- Mode management
- Control logic execution
- Fault detection & tracking
- State transitions
- Logging output

---

## Execution Flow (Per Cycle)

1. Read Inputs  
2. Validate Inputs  
3. Update Mode  
4. Run Control Logic  
5. Update Fault Status  
6. Evaluate System State  
7. Print Logs  

---

## Input Parameters

| Parameter   | Range                             |
|------------|-----------------------------------|
| Speed      | 0 – 200 km/h                      |
| Temperature| -40 – 150 °C                      |
| Gear       | 0 – 5                             |
| Mode       | 0=OFF, 1=ACC, 2=IGNITION_ON       |

---

## How Input is Taken

### Method 1: File Input (Default)

Input file:
system/input.txt

Example:
40 80 1 1
100 120 3 2
130 115 5 2

Format:
Speed Temperature Gear Mode

---

### Method 2: Manual Input (Optional)

Modify input.c and use:
scanf("%d %d %d %d", &speed, &temp, &gear, &mode);

---

## Output Generation

Output file:
system/output.txt

Status file:
system/status.txt

Example Output:

===== ECU CYCLE 1 =====

Fault Analysis:
No faults detected

System State Transition:
Previous: NORMAL -> Current: NORMAL

---

## Directory Structure

ecu_project/
│
├── main.c                  -> Scheduler (entry point)
├── Makefile                -> Build automation
├── readme.md               -> Documentation
│
├── run.sh                  -> Single run script
├── runmul.sh               -> Multiple run script
│
├── ecu_project_exec        -> Compiled executable
│
├── lib/                    -> Core modules
│   ├── input.c / input.h
│   ├── mode.c / mode.h
│   ├── control.c / control.h
│   ├── fault.c / fault.h
│   ├── state.c / state.h
│   ├── log.c / log.h
│   ├── compare.c / compare.h
│   ├── error.c / error.h
│   ├── gen_csv.c
│   └── types.h
│
├── system/                 -> Input/Output files
│   ├── input.txt
│   ├── output.txt
│   ├── status.txt
│   ├── expected.json
│   └── module_size.csv
│
├── perf/                   -> Performance files
│   ├── *.su
│   └── size_raw.txt
│
├── gen_csv/                -> CSV generation

---

## Makefile Usage

Compile:
make

Clean:
make clean

---

## Running the Project

### Single Run

./run.sh

What it does:
- Runs make
- Executes program
- Stores output in system/output.txt

---

### Multiple Runs (Performance)

./runmul.sh

What it does:
- Runs program multiple times
- Finds minimum execution time

---

### Fix Permission Issue

chmod +x run.sh
chmod +x runmul.sh

---

## Control Logic Priority

1. Critical Overheat (>110°C)
2. Invalid Gear
3. Overspeed (>120 km/h)
4. High Temperature (95–110°C)

---

## System States

| State     | Condition                  |
|----------|--------------------------|
| NORMAL   | No faults                |
| DEGRADED | One fault                |
| SAFE     | Multiple/critical faults |

---

## SAFE State Behavior

- System resets
- Mode -> OFF
- Fault counters cleared
- State -> NORMAL

---

## Performance Files (perf/)

- .su files -> Stack usage
- size_raw.txt -> Binary size

---

## Testing

- Boundary values
- Invalid inputs
- Mode transitions
- Fault combinations

---

## Key Features

- Cyclic ECU execution
- Modular design
- Fault prioritization
- Safe-state recovery
- File-based I/O
- Performance tracking
