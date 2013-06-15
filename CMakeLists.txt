# Require at least cmake 2.6.0
cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

# Name of this project
project(HMollerPolGeant4)

# Add the project include directory and put all source files in my_project_sources
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB my_project_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

# Local path for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Custom target that is always built
add_custom_target(QweakSimVersion ALL)

# Creates QweakSimVersion.hh using cmake script
add_custom_command(TARGET QweakSimVersion
  COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
  -P ${CMAKE_CURRENT_SOURCE_DIR}/GetQweakSimVersion.cmake)

# QweakSimVersion.hh is a generated file
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/QweakSimVersion.h
  PROPERTIES GENERATED TRUE
  HEADER_FILE_ONLY TRUE)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Load ROOT and setup include directory
find_package(ROOT REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})

# Create ROOT dictionary before messing up include dirs with Geant4 stuff
# (-p option to "use of the compiler's preprocessor instead of CINT's preprocessor.")
ROOT_GENERATE_DICTIONARY(${CMAKE_CURRENT_BINARY_DIR}/QweakSimDict
  ${CMAKE_CURRENT_SOURCE_DIR}/include/QweakSimUser*Event.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/QweakSimUser*Config.hh
  ${CMAKE_CURRENT_SOURCE_DIR}/include/QweakSimUserRunInformation.hh
  LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/QweakSimLinkDef.hh
  OPTIONS -p)
set(my_project_sources ${my_project_sources} ${CMAKE_CURRENT_BINARY_DIR}/QweakSimDict.cxx)

# Load Geant4 through Find and Use files (the Geant4_DIR directory is where
# the Geant4 cmake Find and Use files can be found)
set(Geant4_DIR $ENV{G4INSTALL}/build)
find_package(Geant4 REQUIRED COMPONENTS gdml qt ui_all vis_all)
include(${Geant4_USE_FILE})

# Link the ROOT and Geant4 libraries
add_library(QweakSim SHARED ${my_project_sources})
add_dependencies(QweakSim QweakSimVersion)
target_link_libraries(QweakSim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

# Add the target executables:
# - QweakSimG4
add_executable(QweakSimG4 QweakSimG4.cc)
target_link_libraries(QweakSimG4 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} QweakSim)
# - QweakSimRoot
add_executable(QweakSimRoot QweakSimRoot.cc)
target_link_libraries(QweakSimRoot ${Geant4_LIBRARIES} ${ROOT_LIBRARIES} QweakSim)
