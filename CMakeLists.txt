cmake_minimum_required(VERSION 3.16)

project(ONNXFront LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Protobuf REQUIRED)

set(ONNX_PROTO ${CMAKE_CURRENT_SOURCE_DIR}/proto/onnx.proto)
protobuf_generate_cpp(ONNX_PROTO_SRCS ONNX_PROTO_HDRS ${ONNX_PROTO})

add_library(onnx_frontend_core
    src/graph.cpp
    src/parse_attr.cpp
    src/exportdot.cpp
    src/onnx_import.cpp
    ${ONNX_PROTO_SRCS}
)


target_include_directories(onnx_frontend_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(onnx_frontend_core PUBLIC protobuf::libprotobuf)

add_executable(frontend src/main.cpp)
target_link_libraries(frontend PRIVATE onnx_frontend_core)


enable_testing()

add_executable(test_graph tests/graph_test.cpp)
target_link_libraries(test_graph onnx_frontend_core)

add_executable(test_parse_attr tests/parse_attr_test.cpp)
target_link_libraries(test_parse_attr onnx_frontend_core)

add_executable(test_import_onnx tests/onnx_import_test.cpp)
target_link_libraries(test_import_onnx onnx_frontend_core)

