# shush::dump
A dumper for my projects.

## Build
```shell
mkdir build && cd build
cmake .. # "-UBUILD_TESTS -DBUILD_TESTS=ON" to build tests, "-ULIBRARY_TYPE -DLIBRARY_TYPE=SHARED" for building shared library
make
```

## How to use
Download the repository and place it into your project directory. In your project's CMakeLists.txt file, insert the following lines:
```cmake
...
add_subdirectory(shush-format)
...
target_link_libraries(${PROJECT_NAME} shush-format)
...
```

## Build documentation
```shell
doxygen
```
Documentation will be generated in the directory named `docs`.