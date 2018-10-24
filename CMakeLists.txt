cmake_minimum_required(VERSION 3.8)
project(Solo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Threads/LockFreeStackByList.h Algorithm/QuickSort.h)

set(Boost_DEBUG 1)
set(BOOST_ROOT "E:\\Boost\\boost_1_64_0")

if (WIN32)
set(Boost_COMPILER "-vc141")
endif()

set(BOOST_LIBRARYDIR "E:\\Boost\\boost_1_64_0\\stage\\lib")
#find_package(Boost 1.68.0 COMPONENTS  thread regex system REQUIRED)
find_package(Boost 1.64.0)
if(Boost_FOUND)
    add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
    add_definitions(-DBOOST_SYSTEM_NO_DEPRECATED)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

include_directories(./Libs/pybind11/include)

add_subdirectory(./Network)
add_subdirectory(./Test)
add_subdirectory(./Threads)
add_subdirectory(./Python)

add_executable(Solo ${SOURCE_FILES})
add_executable(Test ${TEST_FILES})
add_executable(Threads ${THREADS_FILES} Threads/LockFreeStackByListUseHazardPointer.h)

if (Boost_FOUND)
    target_link_libraries(Solo ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY})
    target_link_libraries(Test ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY})
endif()

if (WIN32)
    target_link_libraries(Solo ws2_32 wsock32)
    target_link_libraries(Test ws2_32 wsock32)
endif(WIN32)
