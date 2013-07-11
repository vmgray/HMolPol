# Require at least cmake 2.6.0
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

# Name of this project
project(HMolPol)

# Add the project include directory and put all source files in my_project_sources
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB my_project_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

# Local path for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Load ROOT and setup include directory
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})

# Load Geant4 through Find and Use files (the Geant4_DIR directory is where
# the Geant4 cmake Find and Use files can be found)
set(Geant4_DIR $ENV{G4INSTALL}/build)
find_package(Geant4 REQUIRED COMPONENTS gdml qt ui_all vis_all)
include(${Geant4_USE_FILE})

# Link the ROOT and Geant4 libraries
add_library(HMolPol SHARED ${my_project_sources})
target_link_libraries(HMolPol ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# Add the target executable
add_executable(hmolpol HMolPol.cc)
target_link_libraries(hmolpol ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} HMolPol)

# Update header file
add_custom_command( OUTPUT  ${PROJECT_SOURCE_DIR}/include/gitinfo.hh always COMMAND ${PROJECT_SOURCE_DIR}/pullgitinfo.py ${PROJECT_SOURCE_DIR} )
add_custom_target( gitinfo ALL DEPENDS ${PROJECT_SOURCE_DIR}/include/gitinfo.hh )
