#!/bin/bash

for i in $(seq 1 4); do
    for j in $(seq 1 2); do
        ip="10.0.$i.$j"
        echo "Starting MMS scan attempt to ip $ip..."
        ./mmsScanner $ip
        sleep 1
    done
done