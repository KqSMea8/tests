#!/bin/bash
#ip=192.168.16.28
ip=127.0.0.1
loop_time=100

arr=(4 16 32 64 128 256 512 1024 2048)
for i in "${arr[@]}"
do
    size=$((i*1024))
    echo $size
    ./client ${ip}:40001,${ip}:40002,${ip}:40003,${ip}:40004 $loop_time $size 4
    sleep 1
done
