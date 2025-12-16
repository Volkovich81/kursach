cmake_minimum_required(VERSION 3.16)
project(kursach VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 💡 Находим все .cpp файлы в папке kursach/sources
file(GLOB_RECURSE SOURCES "kursach/sources/*.cpp") 

# 💡 Находим все .h файлы в папке kursach/headers
file(GLOB_RECURSE HEADERS "kursach/headers/*.h")

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Добавляем путь к заголовкам
target_include_directories(${PROJECT_NAME} PRIVATE
    kursach/headers
)

# Опции компилятора
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)