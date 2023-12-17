cmake_minimum_required(VERSION 3.20)

set(TARGET "Titanfall-3.1")
link_libraries(ws2_32 wsock32) # оч надо для сервера

set(HEADERS
    client.h
    draw.h
    environment.h
    err_handle.h
    game_logic.h
    graphics.h
    level_gen.h
    server.h
    settings.h
    ${LIB_DIR}/Graph_lib/fltk.h
    ${LIB_DIR}/Graph_lib/Graph.h
    ${LIB_DIR}/Graph_lib/GUI.h
    ${LIB_DIR}/Graph_lib/Point.h
    ${LIB_DIR}/Graph_lib/Simple_window.h
    ${LIB_DIR}/Graph_lib/Window.h
)
set(SOURCES
    client.cpp
    draw.cpp
    environment.cpp
    err_handle.cpp
    game_logic.cpp
    graphics.cpp
    level_gen.cpp
    main.cpp
    server.cpp
    parser.cpp
    ${LIB_DIR}/Graph_lib/Graph.cpp
    ${LIB_DIR}/Graph_lib/GUI.cpp
    ${LIB_DIR}/Graph_lib/Window.cpp
)

project(${TARGET} CXX)

set(FLTK_SKIP_FLUID True)
set(FLTK_SKIP_FORMS True)

find_package(FLTK REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(SYSTEM ${FLTK_INCLUDE_DIR})
link_directories(${FLTK_INCLUDE_DIR}/../lib)

add_executable(${TARGET} ${HEADERS} ${SOURCES})

target_link_libraries(${TARGET} ${FLTK_LIBRARIES} ${OPENGL_LIBRARIES})
if(WIN32)
target_link_libraries(
    ${TARGET}
    fltk_jpeg
    fltk_png
    fltk_z
)
# if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#     target_link_libraries(${TARGET} fltk_jpeg fltk_png fltk_z)
endif()

install(TARGETS ${TARGET})