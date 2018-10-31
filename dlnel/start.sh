image=bootstrapper:5000/gongweibao/distributed_paddle_ubuntu_grpc:1.1.0
docker run -it --rm ${image} \
    -v /etc/libibverbs.d:/etc/libibverbs.d
    -v /usr/local/nvidia/lib64:/usr/local/nvidia/lib64 \
    -v /usr/local/rdma/lib64:/usr/local/rdma/lib64 \
    -v /usr/lib64/mlnx_ofed/valgrind:/usr/lib64/mlnx_ofed/valgrind
