# 背景
公司内部有一个自己的`mkl`库，由于时代久远(?)现在对其版本已经不可查。公司内部的`CAPI`一直是链接这个库的。后来，换成了`mklml`库，这个时候用户表示`CAPI`库cpu版本的性能下降了1倍到10倍不等。这个project是下降10倍的版本。

# 重现的步骤
## 编译环境
```
docker pull paddlepaddle/centos6u3-capi:latest-dev
```

## 对比时间
修改`CMakeLists.txt`，分别连接`mkl`和`mklml`下的library,然后执行`./build/exeModel`,看其打印的时间作对比。


## 参数
- mklml：
  - Paddle版本: `8b1c50c642914f6ab1fb691059d6d88d9995bea1`
   - 编译参数：
    ```
    cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_TESTING=OFF -DWITH_TEST=OFF -DRUN_TEST=OFF -DWITH_GOLANG=OFF -DWITH_PYTHON=OFF -DWITH_SWIG_PY=OFF -DWITH_DOC=OFF -DWITH_STYLE_CHECK=OFF -DWITH_GPU=OFF -DWITH_AVX=ON -DWITH_C_API=ON -DWITH_MKLDNN=ON -DWITH_MKLML=ON
    ```

- mkl
   - Paddle版本：`43c6ff212e2475b7f39480a9949b53119d332793`
   - 编译参数：
    ```
    cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_TESTING=OFF -DWITH_TEST=OFF -DRUN_TEST=OFF -DWITH_GOLANG=OFF -DWITH_PYTHON=OFF -DWITH_SWIG_PY=OFF -DWITH_DOC=OFF -DWITH_STYLE_CHECK=OFF -DWITH_GPU=OFF -DWITH_AVX=ON -DWITH_C_API=ON -DWITH_MKLDNN=ON -DWITH_MKLML=OFF
    ```