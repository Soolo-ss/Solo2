add_subdirectory(../Libs/pybind11/ build)

set(BUILD_USE_64BITS on)

pybind11_add_module(example example.cpp)
