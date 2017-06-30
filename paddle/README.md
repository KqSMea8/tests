## 构建镜像
```
docker built . -t gongweibao/paddledev
docker push gongweibao/paddledev
```

## Host机器上的准备
- 创建自己的用户

```
useradd -d /home/gongwb -m gongwb  
chown gongwb -R /home/gongwb
su gongwb

mkdir -p ~/go/src/github.com/PaddlePaddle
cd ~/go/src/github.com/PaddlePaddle
git clone https://github.com/gongweibao/Paddle.git
```

## 启动镜像
```
docker run -d -p 2202:22 --name gwbpaddle -it -v ~/go/src/github.com/PaddlePaddle/Paddle:/paddle -v ~/go:/root/go  gongweibao/paddledev
```

## 登陆镜像
```
ssh -p 2202 root@host
```

第一次需要:

```
cd ~/go/src/github.com/PaddlePaddle/Paddle
pre-commit install
vim +GoInstallBinaries +qall
```

## 常用的paddle命令

- 编译安装所有

```
export WITH_GPU=OFF
export WITH_AVX=OFF
export WITH_TEST=ON
cd /paddle
bash paddle/scripts/docker/build.sh
```

- 定制cmake

```
cmake .. \
      -DCMAKE_BUILD_TYPE=Release \
      -DWITH_DOC=OFF \
      -DWITH_GPU=${WITH_GPU:-OFF} \
      -DWITH_AVX=${WITH_AVX:-OFF} \
      -DWITH_GOLANG=${WITH_GOLANG:-OFF} \
      -DWITH_SWIG_PY=ON \
      -DCUDNN_ROOT=/usr/ \
      -DWITH_STYLE_CHECK=${WITH_STYLE_CHECK:-OFF} \
      -DWITH_TESTING=${WITH_TESTING:-OFF} \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make xxxx
```

- [修改python路径](https://www.douban.com/note/334738164/)

可以通过这种方式而不用安装所有的东西达到进行测试部分的目的
```
echo "/root/go/src/github.com/PaddlePaddle/Paddle/python" > /usr/local/lib/python2.7/dist-packages/paddle.pth
```
