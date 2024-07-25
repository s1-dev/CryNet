#!/bin/bash

printf "Attempting to run MasterProgram.py\n"
printf "(Note: Alternatively, this can be achieved by navigating to the master directory manually and running the python script)\n"

cd src/master/

if command -v python3 &> /dev/null
then
    PYTHON_CMD="python3"
else
    PYTHON_CMD="python"
fi

# Run the Python script
$PYTHON_CMD MasterProgram.py
