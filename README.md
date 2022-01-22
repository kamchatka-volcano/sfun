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

#uncomment if you need to install sfun with your target
#set(INSTALL_SFUN ON)
FetchContent_MakeAvailable(sfun)

add_executable(${CMAKE_PROJECT_NAME})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sfun::sfun)
```

For the system-wide installation use these commands:
```
git clone https://github.com/kamchatka-volcano/sfun.git
cd sfun
cmake -S . -B build
cmake --build build
cmake --install build
```

Afterwards, you can use find_package() command to make installed library available inside your project:
```
find_package(sfun 1.0 REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sfun::sfun)
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