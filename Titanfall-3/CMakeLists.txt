# CMakeList.txt: проект CMake для Titanfall-3; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#

# Добавьте источник в исполняемый файл этого проекта.
add_executable (Titanfall-3 "Titanfall-3.cpp" "Titanfall-3.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Titanfall-3 PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
