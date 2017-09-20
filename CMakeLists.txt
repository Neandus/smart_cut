cmake_minimum_required(VERSION 3.2)
project(smart_cut)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV)

add_executable( ${PROJECT_NAME}
    main.cpp
    peopledetector.cpp)

target_link_libraries(${PROJECT_NAME}
    ${OpenCV_LIBS}
    )
