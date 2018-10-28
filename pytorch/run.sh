#!/bin/bash

#source ~/.bash
#source ~/.bashrc

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/:/usr/local/lib/:/workspace/brpc
export CUDA_VISIBLE_DEVICES=0,1,2,3,4,5,6,7

#python main.py -a resnet50 -b 1024 -j 10 --dist-backend gloo --epochs 5 /ssd1/ILSVRC2012/ImageNet/ --world-size 4 --rank 1 --dist-url tcp://xx.xx.xx.xx:20000 --devices 0,1,2,3,4,5,6,7

#which pytorch

#/home/gongwb/soft/paddle_release_home/python/bin/python main.py -a resnet50 -b 512 -j 10 --epochs 5 /home/gongwb/go/dataset/ImageNet --devices 0,1,2,3,4,5,6,7
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib64/:/usr/local/lib/:/workspace/brpc
python main.py -a resnet50 -b 512 -j 20 --epochs 5 /root/go/dataset/ImageNet --devices 0,1,2,3,4,5,6,7


