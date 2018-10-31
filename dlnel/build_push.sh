#!/bin/bash
set -xe

image=bootstrapper:5000/gongweibao/distributed_paddle_ubuntu_grpc:1.1.0
unset http_proxy https_proxy
docker build --network=host --build-arg UBUNTU_MIRROR=http://ftp.sjtu.edu.cn/ubuntu/ . -t ${image}
#docker build  . -t ${image}
docker push http://${image}
