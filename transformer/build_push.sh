#!/bin/bash
set -x
if [[ -z $1 ]]; then
    echo "input rpc name"
    exit 1
fi

rpc_name=$1

image=registry.baidu.com/gongweibao/distributed_paddle:${rpc_name}
docker build --build-arg rpc_name=${rpc_name} --network host . -t ${image}
docker push ${image}

if [[ $rpc_name == "grpc" ]]; then
    rpc_name="nccl2"
    image=registry.baidu.com/gongweibao/distributed_paddle:${rpc_name}
    docker build --build-arg rpc_name=${rpc_name} --network host . -t ${image}
    docker push ${image}
fi
