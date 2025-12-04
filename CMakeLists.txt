cmake_minimum_required(VERSION 3.26)
project(RaspberryPi)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Engine)
set(GAME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Game)
set(EXEC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Executable)

FILE(GLOB_RECURSE EXEC_SOURCES CONFIGURE_DEPENDS "${EXEC_DIR}/src/*.cpp")
add_executable(Executable ${EXEC_SOURCES})
file(COPY "${EXEC_DIR}/shaders/"
     DESTINATION "${CMAKE_BINARY_DIR}/shaders"
)

FILE(GLOB_RECURSE IMGUI_SOURCES CONFIGURE_DEPENDS "${ENGINE_DIR}/ext/imgui/*.cpp")
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ext/imgui)


FILE(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "${ENGINE_DIR}/src/*.cpp")
add_library(Engine STATIC ${ENGINE_SOURCES})
target_include_directories(Engine PUBLIC ${ENGINE_DIR}/src ${ENGINE_DIR}/ext ${ENGINE_DIR}/ext/imgui)
file(COPY "${ENGINE_DIR}/assets/"
     DESTINATION "${CMAKE_BINARY_DIR}/assets"
)

FILE(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS "${GAME_DIR}/src/*.cpp")
add_library(Game STATIC ${GAME_SOURCES})
target_include_directories(Game PUBLIC ${GAME_DIR}/src)
file(COPY "${GAME_DIR}/assets/"
     DESTINATION "${CMAKE_BINARY_DIR}/assets"
)

find_package(Bullet REQUIRED)
find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS EGL)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11 REQUIRED)
    target_link_libraries(Engine PRIVATE X11::X11)
endif()

target_include_directories(Engine PUBLIC 
    ${BULLET_INCLUDE_DIRS}
)
target_link_libraries(Engine PUBLIC assimp ${BULLET_LIBRARIES} imgui OpenGL::EGL)
target_link_libraries(Game PUBLIC Engine)
target_link_libraries(Executable PRIVATE Game)