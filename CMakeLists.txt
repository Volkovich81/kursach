cmake_minimum_required(VERSION 3.16)
project(kursach VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 💡 НОВОЕ: Поиск пакета Qt и необходимых модулей
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Находим все .cpp и .h файлы (остается без изменений)
file(GLOB_RECURSE SOURCES "kursach/sources/*.cpp") 
file(GLOB_RECURSE HEADERS "kursach/headers/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Добавляем путь к заголовкам
target_include_directories(${PROJECT_NAME} PRIVATE
    kursach/headers
)

# 💡 НОВОЕ: Связывание с библиотеками Qt
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

# Опции компилятора
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)