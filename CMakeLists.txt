cmake_minimum_required(VERSION 3.10)

project(deinterlacer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(JPEG REQUIRED)


add_executable(deinterlacer_app
    main.cpp
    deinterlacer.cpp
    jpeg_io.cpp
)


target_include_directories(deinterlacer_app PRIVATE
    ${JPEG_INCLUDE_DIR}
)


target_link_libraries(deinterlacer_app PRIVATE
    ${JPEG_LIBRARIES}
)