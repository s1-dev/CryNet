#!/bin/bash

PROJECT_DIR="src/ircserver/"
PACKAGE_NAME="com.crynet.Main"
CURRENT_DIR=$(pwd)

compile_project() {
    echo "Compiling Maven project in $PROJECT_DIR..."
    cd $PROJECT_DIR
    mvn clean install
    if [ $? -ne 0 ]; then
        echo "Failed to compile the project. Exiting..."
        exit 1
    fi
}

run_project() {
    echo "Running Maven project in $PROJECT_DIR..."
    mvn exec:java -Dexec.mainClass="$PACKAGE_NAME"
    if [ $? -ne 0 ]; then
        echo "Project has exited..."
        exit 1
    fi
}


# Compile and run the project
compile_project
run_project
