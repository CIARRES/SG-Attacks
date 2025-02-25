#!/bin/bash

if [ -z "$N_BATCHES" ]; then
    echo "Error: N_BATCHES environment variable is not set."
    exit 1
fi

intf=${INTF_VAL:-eth0}

./gooseFuzzer $N_BATCHES $intf