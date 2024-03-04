cmake_minimum_required(VERSION 3.2)

project(BoskeopolisLand)
set(OpenGL_GL_PREFERENCE "GLVND")
add_executable(BoskeopolisLand
    controls.cpp
    game.cpp
    gfx.cpp
    glad.c
    io.cpp
    json.c
    json.cpp
    layer.cpp
    level.cpp
    level_inventory.cpp
    level_table.cpp
    log.cpp
    main.cpp
    object_factory.cpp
    sprite.cpp
    sprites/apple.cpp
    sprites/crab.cpp
    sprites/player.cpp
    utility.cpp
)
set_property(TARGET BoskeopolisLand PROPERTY CXX_STANDARD 23)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(include include)
target_link_libraries(BoskeopolisLand ${OPENGL_LIBRARIES} glfw)
file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -ggdb")