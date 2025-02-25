#!/bin/bash

if [ -z "$PM_ID" ]; then
    echo "Error: PM_ID environment variable is not set."
    exit 1
fi

if [ -z "$N_PACKETS" ]; then
    echo "Error: N_PACKETS environment variable is not set."
    exit 1
fi

if [ -z "$IN_VAL" ]; then
    echo "Error: IN_VAL environment variable is not set."
    exit 1
fi

intf=${INTF_VAL:-eth0}

./goosePacketInjector $PM_ID $N_PACKETS $IN_VAL $intf