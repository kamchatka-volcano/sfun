## sfun

**sfun** - is a small header-only library for utility functions that are so small and simple that you define them near the caller and forget to test them.


### Installation

Download and link the library from your project's CMakeLists.txt:
```
cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(sfun
    GIT_REPOSITORY "https://github.com/kamchatka-volcano/sfun.git"
    GIT_TAG "origin/master"
)
FetchContent_MakeAvailable(sfun)

add_executable(my_app)
target_link_libraries(myapp PRIVATE sfun)
```

For the system-wide installation use these commands:
```
git clone https://github.com/kamchatka-volcano/sfun.git
cd sfun
cmake -S . -B build
cmake --build build
cmake --install build
```

### Running tests
```
cd sfun
cmake -S . -B build -DENABLE_TESTS=ON
cmake --build build
cd build/tests && ctest
```

### License
**sfun** is licensed under the [MS-PL license](/LICENSE.md)  