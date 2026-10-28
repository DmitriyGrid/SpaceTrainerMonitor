cmake_minimum_required(VERSION 3.21)
project(SpaceTrainerModules LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 6.2 REQUIRED COMPONENTS Core Widgets Network Sql)
qt_add_executable(SpaceTrainerModules
    src/main.cpp src/MainWindow.cpp include/MainWindow.h
    src/TelemetryServer.cpp include/TelemetryServer.h
    src/EquipmentSimulator.cpp include/EquipmentSimulator.h
    src/DatabaseManager.cpp include/DatabaseManager.h)
target_link_libraries(SpaceTrainerModules PRIVATE Qt6::Core Qt6::Widgets Qt6::Network Qt6::Sql)

target_include_directories(SpaceTrainerModules PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
