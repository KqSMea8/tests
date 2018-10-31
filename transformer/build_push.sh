#!/bin/bash
set -x
if [[ -z $1 ]]; then
    echo "input rpc name"
    exit 1
fi

rpc_name=$1

rpc_image=registry.baidu.com/gongweibao/distributed_paddle:${rpc_name}
docker build --build-arg rpc_name=${rpc_name} --network host . -t ${rpc_image}
docker push ${image}

if [[ $rpc_name == "grpc" ]]; then
    nccl2_image=registry.baidu.com/gongweibao/distributed_paddle:nccl2
    docker tag ${rpc_image} ${nccl2_image}
    docker push ${nccl2_image}
fi
