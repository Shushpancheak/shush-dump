# shush::dump
A dumper for my projects.

## Build
```shell
mkdir build && cd build
cmake .. # "-UBUILD_TESTS -DBUILD_TESTS=ON" to build tests, "-ULIBRARY_TYPE -DLIBRARY_TYPE=SHARED" for building shared library
make
```

## Usage
Download the repository and place it into your project directory. In your project's CMakeLists.txt file, insert the following lines:
```cmake
...
add_subdirectory(shush-dump)
...
target_link_libraries(${PROJECT_NAME} shush-dump)
...
```

Using the dumper implies that for every class you should decalre `void Ok();` for checking the state of the object which will have `MASSERT(expression, error_code)`s (work only in debug) and `UMASSERT`s. Also, a class should have a `char* GetDumpMessage(int error_code)` which will return a pointer to a string that will be showed in the dump file.
Prefer static container for message strings for this dumper was designed with purpose of not using dynamic allocation whatsoever. For that matter, you also should add `setbuf(stdout, NULL);` in the beggining of your program.

## Build documentation
```shell
doxygen
```
Documentation will be generated in the directory named `docs`.