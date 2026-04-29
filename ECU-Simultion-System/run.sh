#!/bin/bash

echo "Cleaning..."
make clean

echo "Building..."
make

echo "Running..."
./ecu_project_exec