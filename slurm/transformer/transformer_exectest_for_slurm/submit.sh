#!/bin/bash

HGCP_CLIENR_BIN=~/.hgcp/software-install/HGCP_client/bin

${HGCP_CLIENR_BIN}/submit \
        --hdfs afs://xingtian.afs.baidu.com:9902 \
        --hdfs-user COMM_KM_Data \
        --hdfs-passwd COMM_km_2018 \
        --hdfs-path /user/COMM_KM_Data/gongweibao/$1 \
        --file-dir ./ \
        --job-name $1 \
        --queue-name yq01-p40-4-8 \
        --num-nodes 3 \
        --num-task-pernode 1 \
        --gpu-pnode 8 \
        --job-script ./job.sh

#        --queue-name yq01-p40-3-8 \
#        --queue-name yq01-v100-box-1-8 \
#        --queue-name yq01-p40-4-8 \
