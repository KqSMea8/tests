#!/bin/bash
ip=192.168.16.28
loop_time=512

arr=(4 16 32 64 128 256 512 1024 2048)
for i in "${arr[@]}"
do
    size=$((i*1024))
    echo $size
nohup ./client ${ip}:40001 $loop_time $size 1 > 40001_$size.log 2>&1 &
nohup ./client ${ip}:40002 $loop_time $size 1 > 40002_$size.log 2>&1 &
nohup ./client ${ip}:40003 $loop_time $size 1 > 40003_$size.log 2>&1 &
nohup ./client ${ip}:40004 $loop_time $size 1 > 40004_$size.log 2>&1 &
	sleep 3s
done
