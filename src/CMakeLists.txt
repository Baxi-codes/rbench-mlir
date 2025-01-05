configure_file(config.h.in config.h)

add_executable(server server.cpp)
target_link_libraries(server gRPC::grpc++ rbench_proto)
target_include_directories(server PUBLIC ${CMAKE_CURRENT_BINARY_DIR})