```shell
source /opt/qnx/qnx700/qnxsdp-env.sh
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain.cmake ..
make
```