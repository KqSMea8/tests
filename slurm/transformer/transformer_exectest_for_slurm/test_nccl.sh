#!/bin/bash

echo ${iplist}
echo "hello"
hostname

export PATH=$PWD/python/bin/:$PATH
export LD_LIBRARY_PATH=/home/work/cuda-9.0/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/work/cudnn/cudnn_v7/cuda/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$PWD/nccl_2.3.5/lib/:$LD_LIBRARY_PATH

# wangxi add for slurm
export PADDLE_DATASET_DIR=$PWD/dataset/

export PADDLE_PSERVER_PORT=9184
export PADDLE_TRAINER_IPS=${iplist} 
#export PADDLE_CURRENT_IP=`host $HOSTNAME | awk '{print $4}'`
export PADDLE_CURRENT_IP=`/sbin/ip a | grep inet | grep global | awk '{print $2}' | sed 's/\/[0-9][0-9].*$//g'`

echo "ip="${PADDLE_CURRENT_IP}

#export PADDLE_TRAINER_ID=${OMPI_COMM_WORLD_RANK}

iparray=(${iplist//,/ })
for i in "${!iparray[@]}"; do
    if [ ${iparray[$i]} == ${PADDLE_CURRENT_IP} ]; then
        export PADDLE_TRAINER_ID=$i
    fi
done

#echo ${PADDLE_TRAINER_ID}

# paddle debug
#export GLOG_v=7
#export GLOG_logtostderr=1

# nccl debug
#export NCCL_DEBUG=INFO

#export NCCL_IB_DISABLE=1
#export NCCL_IB_GDR_LEVEL=4
export NCCL_IB_GID_INDEX=3

model=$1
bsize=$2
gpus=$3

#which python 
python fluid_benchmark.py \
        --no_test \
        --model $model \
        --batch_size $bsize \
        --pass_num 1 \
        --iterations 30 \
        --device GPU \
        --gpus $gpus \
        --update_method nccl2 &> log/${PADDLE_TRAINER_ID}.txt

