#!/bin/bash
#pkill -f server
#sleep 3

nohup ./server 40001 2 > 40001.log 2>&1 &
nohup ./server 40002 2 > 40002.log 2>&1 &
nohup ./server 40003 2 > 40003.log 2>&1 &
nohup ./server 40004 2 > 40004.log 2>&1 &
