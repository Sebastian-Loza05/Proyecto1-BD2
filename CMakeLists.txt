cmake_minimum_required(VERSION 3.20)
project(PROYECTO1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(PROYECTO1
        datos.csv
        main.cpp
        methods.h
        SequentialFile.h Structures/AVLFile.cpp Structures/AVLFile.cpp Structures/functions.h Structures/methods.h Structures/Record.h)


if (${BUILD_QDS_COMPONENTS})
    include(${CMAKE_CURRENT_SOURCE_DIR}/GUI/GUI/qmlcomponents)
endif ()

