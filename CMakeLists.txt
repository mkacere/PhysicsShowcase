cmake_minimum_required(VERSION 3.12)
project(PhysicsShowcase)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Find SFML
find_package(SFML REQUIRED COMPONENTS graphics window system)

# Check if SFML was found
if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML not found. Please install SFML or provide the path to SFML in CMake.")
endif()

# Include headers
include_directories(PhysicsShowcase PRIVATE src)
include_directories(PhysicsShowcase PRIVATE include)  # Add this line for the "include" directory


# Add all source files
file(GLOB SOURCES "src/*.cpp")

# Create .exe
add_executable(PhysicsShowcase ${SOURCES})

# Link SFML
target_link_libraries(PhysicsShowcase PRIVATE sfml-graphics sfml-window sfml-system)
