#!/bin/bash

set -x

source /home/gongwb/application/bin/paddlecloud_1.0.3/demo/command-test/config.sh 


paddlecloud job \
    --server ${server} \
    --port ${port} \
    --user-ak ${user_ak} \
    --user-sk ${user_sk} \
    train --job-version paddle-fluid-custom \
    --k8s-gpu-type baidu/gpu_p40 \
    --cluster-name paddle-jpaas-ai00-gpu \
    --k8s-gpu-cards 8 \
    --k8s-priority high \
    --k8s-memory 60Gi \
    --k8s-ps-memory 30Gi \
    --job-name gongweibao-transformer \
    --start-cmd "python run.py" \
    --job-conf transformer/conf.py \
    --files transformer/config.py transformer/infer.py transformer/model.py transformer/optim.py transformer/profile.py transformer/reader.py transformer/train.py transformer/util.py transformer/run.py \
    --k8s-not-local  \
    --k8s-trainers 2 \
    --k8s-cpu-cores 7 \
    --k8s-ps-num 2 \
    --k8s-ps-cores 7 \
    --image-addr "registry.baidu.com/gongweibao/distribute_test_transformer:latest"
