cmake_minimum_required(VERSION 3.14)
project(Reversi VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
add_executable(Reversi
    main.cpp
    GameEngine.h
    GameEngine.cpp
    MainWindow.h
    MainWindow.cpp
    MenuWidget.h
    MenuWidget.cpp
    GameWidget.h
    GameWidget.cpp
    CellButton.h
    CellButton.cpp
)
target_link_libraries(Reversi Qt5::Widgets)
