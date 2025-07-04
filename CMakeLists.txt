cmake_minimum_required(VERSION 3.16)
project(ZoraPerl)

set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64") # Adjust to your Qt install

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(ZoraPerl
    zora_perl_main.cpp
    system_checker.cpp
    system_checker.h
    python_manager.cpp
    python_manager.h
    desktop_environment.cpp
    desktop_environment.h
)

target_link_libraries(ZoraPerl
    Qt6::Widgets
)

# Set the executable to be placed in the ZoraPerl directory
set_target_properties(ZoraPerl PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)