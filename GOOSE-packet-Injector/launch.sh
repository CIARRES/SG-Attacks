#!/bin/bash

if [ -z "$N_BATCHES" ]; then
    echo "Error: N_BATCHES environment variable is not set."
    exit 1
fi

intf=${INTF_VAL:-eth0}

for id in $(seq 1 4); do
    for val in $(seq 0 1); do
        echo "Starting GOOSE packet injection to PM $id..."
        ./goosePacketInjector $id $N_BATCHES $val $intf
        sleep 1
    done
done