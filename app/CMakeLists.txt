project(app)

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package (gflags REQUIRED)

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/freetype-2.10.2/include
${CMAKE_CURRENT_SOURCE_DIR}/../common
${CMAKE_CURRENT_SOURCE_DIR}/../image
${CMAKE_CURRENT_SOURCE_DIR}/../freetype
)
include_directories (${gflags_INCLUDE_DIR})

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/freetype-2.10.2/lib)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../common SRCS_common)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../image SRCS_image)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/../freetype SRCS_freetype)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/show_stock SRCS_show_stock)

add_executable(show_stock
${SRCS_image}
${SRCS_common}
${SRCS_freetype}
${SRCS_show_stock}
)
target_link_libraries(show_stock
${OpenCV_LIBS}
gflags
freetype
libcnpy.a
pthread
z)

add_subdirectory(test)
