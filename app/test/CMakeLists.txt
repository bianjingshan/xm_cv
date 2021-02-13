project(examples)
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/freetype-2.10.2/include
${CMAKE_CURRENT_SOURCE_DIR}/../../common
${CMAKE_CURRENT_SOURCE_DIR}/../../image
${CMAKE_CURRENT_SOURCE_DIR}/../../freetype
)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/freetype-2.10.2/lib)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../../common SRCS_common)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../../image SRCS_image)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../../freetype SRCS_freetype)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/freetype_test SRCS_freetype_test)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/log_test SRCS_log_test)

add_executable(freetype_test
${SRCS_image}
${SRCS_common}
${SRCS_freetype}
${SRCS_freetype_test}
)
target_link_libraries(freetype_test
${OpenCV_LIBS}
freetype
pthread
z)

add_executable(log_test
${SRCS_common}
${SRCS_log_test}
)
