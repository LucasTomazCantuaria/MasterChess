cmake_minimum_required (VERSION 3.0)

set(UnityDir "${CMAKE_SOURCE_DIR}/Unity/")

set(UnityFile "UnityRpc")

set(UnitySources
    "${UnityFile}.grpc.pb.h" "${UnityFile}.grpc.pb.cc"
    "${UnityFile}.pb.h" "${UnityFile}.pb.cc"
)

if (WIN32)
    set(PluginPath "F:/vcpkg/installed/x64-windows/tools/grpc/grpc_cpp_plugin.exe")
    set(IncPath "F:/vcpkg/installed/x64-windows/include/")
endif()

list(TRANSFORM UnitySources PREPEND "${UnityDir}generated/")

add_custom_command(
    OUTPUT ${UnitySources}
    COMMAND protoc "${UnityFile}.proto" --grpc_out="${UnityDir}generated" --cpp_out="${UnityDir}generated" --plugin=protoc-gen-grpc=${PluginPath} --proto_path=. --proto_path=${IncPath}
    WORKING_DIRECTORY ${UnityDir}
)

add_custom_target(UnityGrpc DEPENDS ${UnitySources})


add_library(Unity STATIC
    "UnityGameListener.hpp" "UnityGameListener.cpp"
    ${UnitySources}
)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(Unity PRIVATE gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++)
