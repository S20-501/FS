# FS

Filesystem RT-11.

Clone project:
```
git clone --recurse-submodules https://github.com/S20-501/FS
```

Build project:

Old CMake (3.5):
```
mkdir -p bin/cmake-build-debug && cd bin/cmake-build-debug/ && cmake ../.. && make
```

New CMake (since 3.13):
```
cmake -B bin/cmake-build-debug/ && cd bin/cmake-build-debug/ && make
```
