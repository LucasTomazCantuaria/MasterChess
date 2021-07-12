cmake_minimum_required (VERSION 3.0)

set(UnityDir "${CMAKE_SOURCE_DIR}/Unity/")

set(UnityFile "UnityRpc")

set(UnitySources
    "${UnityFile}.grpc.pb.h" "${UnityFile}.grpc.pb.cc"
    "${UnityFile}.pb.h" "${UnityFile}.pb.cc"
)

list(TRANSFORM UnitySources PREPEND "${UnityDir}generated/")

add_custom_command(
    OUTPUT ${UnitySources}
    COMMAND powershell ./gen.ps1 ${UnityFile} generated
    WORKING_DIRECTORY ${UnityDir}
)

add_custom_target(UnityGrpc DEPENDS ${UnitySources})


add_library(Unity STATIC
    "UnityGameListener.hpp" "UnityGameListener.cpp"
    ${UnitySources}
)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(Unity PRIVATE gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++)
