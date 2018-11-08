#!/bin/bash
set -x

mpirun hostname

echo "start"

iplist=`cat nodelist-${SLURM_JOB_ID} | xargs  | sed 's/ /,/g'`
mpirun --bind-to none -x iplist=${iplist} sh test_nccl.sh transformer 3200 8

#mpirun --bind-to none -x iplist=${iplist} sh test_nccl.sh resnet 32 8
#mpirun --bind-to none -x iplist=${iplist} sh test_pserver.sh resnet 32 8 &> log/resnet_32_8.txt
#mpirun --bind-to none sh test_local.sh resnet 32 8 &> log/resnet_32_8.txt

