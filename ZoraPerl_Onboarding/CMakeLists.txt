cmake_minimum_required(VERSION 3.16)
project(ZoraPerl_Onboarding)

set(CMAKE_AUTOMOC ON)
set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64") # Adjust to your Qt install

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

# Tell CMake to compile the Qt resource file
qt_add_resources(RESOURCES resources.qrc)

add_executable(ZoraPerl_Onboarding
    main.cpp
    softlanding.cpp
    softlanding.h
    setupwizard.cpp
    setupwizard.h
    ${RESOURCES}  # Include the compiled resource
)

target_link_libraries(ZoraPerl_Onboarding
    Qt6::Widgets
    Qt6::Multimedia
)
