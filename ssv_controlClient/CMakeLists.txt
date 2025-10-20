cmake_minimum_required(VERSION 3.13)
project(ControlClient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(vsomeip3 3.4.10 REQUIRED)
find_package(Boost 1.55 COMPONENTS system thread log REQUIRED)

include_directories(
	${Boost_INCLUDE_DIR}
	${VSOMEIP_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

file(GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

file(COPY config/vsomeip.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(app ${SRC_FILES})
target_link_libraries(app vsomeip3 ${Boost_LIBRARIES})