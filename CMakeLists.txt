cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DNOMINMAX)

project("Master Chess")

include_directories(BEFORE .)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
set(PROC_LIBS Threads::Threads)

set(ConChessLibs MasterChess Math Arduino Unity)

if(WIN32)
    find_package(fmt CONFIG REQUIRED)
    find_package(tiny-process-library CONFIG REQUIRED)
    set(PROC_LIBS ${PROC_LIBS} fmt::fmt tiny-process-library::tiny-process-library)
    set(ConChessLibs ${ConChessLibs} fmt::fmt tiny-process-library::tiny-process-library)
    add_subdirectory("Windows")
    set(ConChessLibs ${ConChessLibs} Windows)
else()
    add_subdirectory("Linux")
    set(PROC_LIBS ${PROC_LIBS} -lfmt -ltiny-process-library -lwiringPi)
endif()

add_subdirectory("Math")

add_subdirectory("MasterChess")

add_subdirectory("Arduino")

add_subdirectory("Unity")

add_executable(ConChess "Source.cpp")
target_link_libraries(ConChess PRIVATE ${ConChessLibs})
