#!/bin/bash

echo "Cleaning..."
make clean

echo "Building..."
make

min=999999999

echo "Running multiple times..."

for i in {1..5}
do
    echo "Run $i"

    output=$(./ecu_project_exec)

    # Extract LAST numeric value (avg or total)
    cycles=$(echo "$output" | grep "read_inputs" | awk '{print $4}' | head -1)

    if [[ -z "$cycles" ]]
    then
        echo "⚠ Skipping invalid run"
        continue
    fi

    echo "Cycles: $cycles"

    if [ "$cycles" -lt "$min" ]
    then
        min=$cycles
    fi

done

echo "=========================="
echo "MIN CPU CYCLES: $min"
echo "=========================="