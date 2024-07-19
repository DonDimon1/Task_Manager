cmake_minimum_required(VERSION 3.20)#3.20

project(task_manager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        taskdialog.h
        taskdialog.cpp
        taskdialog.ui
        tasksqltablemodel.h
        tasksqltablemodel.cpp
)

add_executable(task_manager ${PROJECT_SOURCES})
target_link_libraries(task_manager PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Sql)

