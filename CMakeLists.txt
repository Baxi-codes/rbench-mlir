cmake_minimum_required(VERSION 3.20.0)

project(RBench LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

find_package(MLIR REQUIRED CONFIG)

message(STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/bin)
set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/lib)
set(MLIR_BINARY_DIR ${CMAKE_BINARY_DIR})

list(APPEND CMAKE_MODULE_PATH "${MLIR_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(TableGen)
include(AddLLVM)
include(AddMLIR)
include(HandleLLVMOptions)

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${MLIR_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR}/include)
link_directories(${LLVM_BUILD_LIBRARY_DIR})
add_definitions(${LLVM_DEFINITIONS})

get_property(dialect_libs GLOBAL PROPERTY MLIR_DIALECT_LIBS)
get_property(conversion_libs GLOBAL PROPERTY MLIR_CONVERSION_LIBS)
set(LIBS
	${dialect_libs}
	${conversion_libs}
)

option(
	GRPC_PLUGIN_PATH
	"Path to grpc_cpp_plugin. You can get it by doing `which grpc_cpp_plugin`"
	""
)

if(GRPC_PLUGIN_PATH STREQUAL "")
    message(FATAL_ERROR "GRPC_PLUGIN_PATH must be specified.")
endif()

option(
	FILE_UPLOAD_PATH
	"Path to directory that would store the uploaded files."
	"${CMAKE_CURRENT_SOURCE_DIR}/uploads"
)

add_library(rbench_proto proto/rbench.proto)
target_link_libraries(rbench_proto gRPC::grpc++)
target_include_directories(rbench_proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate(TARGET rbench_proto LANGUAGE cpp)
protobuf_generate(
	TARGET rbench_proto
	LANGUAGE grpc
	GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
	PLUGIN "protoc-gen-grpc=${GRPC_PLUGIN_PATH}"
)

add_subdirectory(src)