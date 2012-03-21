#!/usr/bin/env bash
MAX=1500

for x in `seq $MAX`; do
    echo "Processing $x/$MAX"
    ./trainer
    cat weights.txt
done
