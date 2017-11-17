#/bin/bash

set -e

mkdir -p build

#link mklml library
sed -i -e "s#^link_directories.*#link_directories\(\${CMAKE_SOURCE_DIR}/mklml/lib\)#g" CMakeLists.txt
rm -rf build/* && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd ..
cp build/exeModel ./mklml_exe


#link mkl library
sed -i -e "s#^link_directories.*#link_directories\(\${CMAKE_SOURCE_DIR}/mkl/lib\)#g" CMakeLists.txt
rm -rf build/* && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd ..
cp build/exeModel ./mkl_exe


