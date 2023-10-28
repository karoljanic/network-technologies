#!/bin/bash

source_file="main.cpp"
output_file="main"

compiler_options="-std=c++17 -Wall -Wextra"
linker_options="-lboost_system"

g++ $compiler_options -o $output_file $source_file $linker_options
