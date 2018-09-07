#!/bin/bash
cat 1.log | grep CPU | awk -F"[(,]" '{print $4}' | awk -F "[=]" '{print $2}'
