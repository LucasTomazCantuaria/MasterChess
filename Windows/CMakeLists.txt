cmake_minimum_required (VERSION 3.0)

find_package(fmt CONFIG REQUIRED)

set(Windows-Targets
    "Console.hpp" "Console.cpp"
)

add_library(Windows STATIC ${Windows-Targets})
target_link_libraries(Windows PRIVATE fmt::fmt)
