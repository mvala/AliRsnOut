#!/bin/bash

PROJECT_DIR="$(dirname $(dirname $(readlink -m $0)))"
export LD_LIBRARY_PATH="$PROJECT_DIR/lib:$LD_LIBRARY_PATH"
export PATH="$PROJECT_DIR/bin:$PATH"
