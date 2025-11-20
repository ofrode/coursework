cmake_minimum_required(VERSION 3.16)
project(PsychologicalTests VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Находим Qt6 (или Qt5, если Qt6 не установлен)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

# Включаем автоматическую обработку MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Добавляем директории с заголовочными файлами
include_directories(${CMAKE_SOURCE_DIR}/hdr)
include_directories(${CMAKE_SOURCE_DIR}/hdr/core)
include_directories(${CMAKE_SOURCE_DIR}/hdr/ui)
include_directories(${CMAKE_SOURCE_DIR}/hdr/utils)

# Список всех исходных файлов
set(SOURCES
    src/main.cpp
    # Core classes
    src/core/Answer.cpp
    src/core/Question.cpp
    src/core/Test.cpp
    src/core/TestResult.cpp
    src/core/Statistics.cpp
    # UI classes (заголовочные файлы для MOC)
    hdr/ui/MainWindow.h
    hdr/ui/AddTestWindow.h
    hdr/ui/TestWindow.h
    hdr/ui/ResultsWindow.h
    hdr/ui/StatisticsWindow.h
    # UI classes (исходные файлы)
    src/ui/MainWindow.cpp
    src/ui/AddTestWindow.cpp
    src/ui/TestWindow.cpp
    src/ui/ResultsWindow.cpp
    src/ui/StatisticsWindow.cpp
    # Utils
    src/utils/FileManager.cpp
    src/utils/ColorPalette.cpp
)

# Добавляем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Связываем с Qt библиотеками
target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

# Настройки для установки (опционально)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)


