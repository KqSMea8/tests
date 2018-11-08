#!/bin/bash

export PATH=$PWD/python/bin/:$PATH

export LD_LIBRARY_PATH=/home/work/cuda-9.0/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/work/cudnn/cudnn_v7/cuda/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$PWD/nccl_2.3.5/lib/:$LD_LIBRARY_PATH

export PADDLE_DATASET_DIR=$PWD/dataset/

model=$1
bsize=$2
gpus=$3

python fluid_benchmark.py \
        --no_test      \
        --model $model \
        --batch_size $bsize \
        --pass_num 10 \
        --iterations 30 \
        --device GPU \
        --gpus $gpus \
