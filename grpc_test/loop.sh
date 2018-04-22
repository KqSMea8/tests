#!/bin/bash


#for ((i=1024;i<=10489760;i=i*2))
#do
#    for m in {1..20}
#    do
#        ./client 0.0.0.0 50001 $i  > 50001.log &
#        ./client 0.0.0.0 50002 $i  > 50002.log &
#    done
#done


#for i in {10..11}
#do
#    ./server 500$i > /dev/null &
#    ./client 0.0.0.0 500$i 1048576  > $i.log &
#done

nohup ./server 5001 4 > /dev/null &
sleep 3s

arr=(4 16 32 64 128 256 512 1024 2048 4096 81024 )
for i in "${arr[@]}"
do
    size=$((i*1024))
    echo $size
    ./client 127.0.0.1:5001 300 $size
done


