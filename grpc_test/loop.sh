#!/bin/bash


#for ((i=1024;i<=10489760;i=i*2))
#do
#    for m in {1..20}
#    do
#        ./client 0.0.0.0 50001 $i  > 50001.log &
#        ./client 0.0.0.0 50002 $i  > 50002.log &
#    done
#done


for i in {10..11}
do
    ./server 500$i > /dev/null &
    ./client 0.0.0.0 500$i 1048576  > $i.log &
done

