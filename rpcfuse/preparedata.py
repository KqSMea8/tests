from recordio_converter import *

path="/root/.cache/paddle/dataset/recordio"
prepare_mnist(path, 64);
prepare_flowers(path, 64);
prepare_cifar10(path, 64);
