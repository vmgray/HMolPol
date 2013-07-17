#GetHMolPolVersion.cmake

# Created on: Jul 16, 2013
#     Author: vmgray

# Local path for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

# Find Git 
find_package(Git REQUIRED)
#GIT_WC_INFO(${CMAKE_SOURCE_DIR} HMOLPOL)

# Write version info to local text file
file(WRITE HMolPolVersion.txt "// This file is generated automatically.  All changes will be lost.\n")
#file(APPEND HMolPolVersion.txt "#define HMOLPOL_URL   \"${HMOLPOL_WC_URL}\"\n")
#file(APPEND HMolPolVersion.txt "#define HMOLPOL_ROOT    \"${HMOLPOL_WC_ROOT}\"\n")
#file(APPEND HMolPolVersion.txt "#define HMOLPOL_REVISION  \"${HMOLPOL_WC_REVISION}\"\n")
#file(APPEND HMolPolVersion.txt "#define HMOLPOL_REVISION_NAME  \"${HMOLPOL_WC_REVISION_NAME}\"\n")
#file(APPEND HMolPolVersion.txt "#define HMOLPOL_REVISION_HASH  \"${HMOLPOL_WC_REVISION_HASH}\"\n")
#file(APPEND HMolPolVersion.txt "#define HMOLPOL_INFO   \"${HMOLPOL_WC_INFO}\"\n")
#file(APPEND HMolPolVersion.txt "#define HMOLPOL_LAST_CHANGED_AUTHOR \"${HMOLPOL_WC_LAST_CHANGED_AUTHOR}\"\n")
#file(APPEND HMolPolVersion.txt "#define HMOLPOL_LAST_CHANGED_DATE \"${HMOLPOL_WC_LAST_CHANGED_DATE}\"\n")
#file(APPEND HMolPolVersion.txt "#define HMOLPOL_LAST_CHANGED_REV  \"${HMOLPOL_WC_LAST_CHANGED_REV}\"\n")
#file(APPEND HMolPolVersion.txt "#define HMOLPOL_LAST_CHANGED_LOG \"${HMOLPOL_LAST_CHANGED_LOG}\"\n")
# git log -n 1 && git status -bs

# Copy only when file changes
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different
  HMolPolVersion.txt HMolPolVersion.hh)
