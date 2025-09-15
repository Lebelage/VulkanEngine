cmake_minimum_required(VERSION 3.16)
project(pme_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_DIR "${CMAKE_SOURCE_DIR}/external/glfw")

# Поиск библиотеки GLFW
if(WIN32)
    find_library(GLFW_LIBRARY 
        NAMES glfw3 glfw
        PATHS "${GLFW_DIR}/lib/windows"
        REQUIRED
    )

elseif(APPLE)
    find_library(GLFW_LIBRARY 
        NAMES glfw3 glfw
        PATHS "${GLFW_DIR}/lib/macos"
        REQUIRED
    )
endif()

find_package(Vulkan REQUIRED)
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan SDK not found.")
endif()
    message("Vulkan SDK path: ${Vulkan_INCLUDE_DIRS}")

file(GLOB_RECURSE SOURCES "src/*.cpp")


# Добавляем исполняемый файл
add_executable(${PROJECT_NAME}
    ${SOURCES}
)

# Подключаем include директории
target_include_directories(${PROJECT_NAME} PRIVATE
    src/core/include
    src/utils/include
    "${GLFW_DIR}/include"
)

# Линковка библиотек
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${GLFW_LIBRARY}
    Vulkan::Vulkan
)
