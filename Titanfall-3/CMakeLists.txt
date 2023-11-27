
set(TARGET "Titanfall-3")
    link_libraries(ws2_32 wsock32) # оч надо для сервера

project(${TARGET} LANGUAGES CXX)
add_executable(
        ${TARGET}
        main.cpp
        enviroment.cpp
        level_gen.cpp
        server.cpp
        client.cpp
        err_handle.cpp
)

install(TARGETS ${TARGET})
