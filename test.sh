#!/bin/bash

# Exit on error
set -e

# Define paths and commands
SRC_DIR="./src"
GEN_SCRIPT="$SRC_DIR/generate_header.sh"
GEN_FILES="$SRC_DIR/gen/files.h"
GEN_HEADER="$SRC_DIR/gen/src/header.h"
GEN_RULES="$SRC_DIR/gen/src/rules.h"
MAKE_CMD="make"
TEST_INPUT_DIR="tests/"
TEST_OUTPUT_DIR="tests/output/"
MAIN_BIN="$TEST_OUTPUT_DIR/bin/main"
MAIN_SOURCE="$TEST_OUTPUT_DIR/main.S"

echo "Generating header files..."
$GEN_SCRIPT $GEN_FILES $GEN_HEADER $GEN_RULES


echo "Running make..."
$MAKE_CMD all

echo "Executing main binary..."
output/main -i $TEST_INPUT_DIR -o $TEST_OUTPUT_DIR

echo "Preprocessing assembly source..."
gcc -o $MAIN_BIN $MAIN_SOURCE -Lc

echo "Cleaning..."
$MAKE_CMD clean

echo "Setting executable permissions on $MAIN_BIN..."
chmod +x $MAIN_BIN

echo "System architecture:"
uname -i

echo "Running the binary..."
$MAIN_BIN
