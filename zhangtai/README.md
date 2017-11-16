# 背景
公司内部有一个自己的`mkl`库，由于时代久远(?)现在对其版本已经不可查。公司内部的`CAPI`一直是链接这个库的。后来，换成了`mklml`库，这个时候用户表示`CAPI`库cpu版本的性能下降了1倍到10倍不等。这个project是下降10倍的版本。

# 重现的步骤
## 编译环境
```
docker pull paddlepaddle/centos6u3-capi
```

## 如何做对比
现在这个repository里边连接的是`mkl`版本，可以换成链接`mklml`的，然后看其打印的时间作对比。
