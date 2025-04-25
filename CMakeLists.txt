cmake_minimum_required(VERSION 3.14)

project(Reversi VERSION 1.0 LANGUAGES CXX)

# Задаём стандарт C++ (например, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем автоматическую обработку moc, uic и rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Находим необходимые компоненты Qt (здесь используется Qt5 Widgets)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Добавляем исполняемый файл, перечисляя все исходники проекта
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

# Линкуем проект с библиотекой Qt5 Widgets
target_link_libraries(Reversi Qt5::Widgets)
