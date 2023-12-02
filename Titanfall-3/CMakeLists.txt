
set(TARGET "Titanfall-3")
    link_libraries(ws2_32 wsock32) # оч надо для сервера

project(${TARGET} LANGUAGES CXX)
add_executable(
        ${TARGET}
        main.cpp
        environment.cpp
        level_gen.cpp
        server.cpp
        client.cpp
        err_handle.cpp
        graphics.cpp
        # sounds.cpp
)

install(TARGETS ${TARGET})