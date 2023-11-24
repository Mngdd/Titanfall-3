
set(TARGET "Titanfall-3")
project(${TARGET} LANGUAGES CXX)
add_executable(
    ${TARGET}
    main.cpp
    enviroment.cpp
    level_gen.cpp
    server.cpp
)
install(TARGETS ${TARGET})