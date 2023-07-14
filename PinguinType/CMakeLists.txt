cmake_minimum_required(VERSION 3.20)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET_NAME "PinguinType")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Sql)

file(GLOB_RECURSE SRC "SourceFiles/*.cpp" "SourceFiles/*.h" "SourceFiles/*.ui" "*.qrc")

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets Qt::Gui Qt6::Sql)
