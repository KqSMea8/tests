#!/bin/bash
image=bootstrapper:5000/gongweibao/fluid_benchmark_grpc_gpu:0.15.0
docker build --build-arg UBUNTU_MIRROR=http://ftp.sjtu.edu.cn/ubuntu/ . -t ${image}
docker push ${image}
