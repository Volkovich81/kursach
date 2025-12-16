cmake_minimum_required(VERSION 3.16)
project(kursach VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Находим все .cpp и .h файлы 
file(GLOB_RECURSE SOURCES "kursach/sources/*.cpp") 
file(GLOB_RECURSE HEADERS "kursach/headers/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 💡 ИСПРАВЛЕНИЕ: Мы добавляем в пути корневую папку kursach
# Если ваш код использует #include "headers/HallDialog.h", то CMake должен искать 
# папку 'headers' в 'kursach'.
target_include_directories(${PROJECT_NAME} PRIVATE
    kursach
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)