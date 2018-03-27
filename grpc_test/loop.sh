#!/bin/bash

for ((i=1024;i<=10489760;i=i*2))
do
    for m in {1..20}
    do
        ./client 0.0.0.0 50051 $i 
    done
done
