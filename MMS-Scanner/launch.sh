#!/bin/bash

if [ -z "$IP" ]; then
    echo "Error: IP environment variable is not set."
    exit 1
fi

./mmsScanner $IP