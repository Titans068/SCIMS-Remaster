# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\SCIMS_Remastered_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\SCIMS_Remastered_autogen.dir\\ParseCache.txt"
  "SCIMS_Remastered_autogen"
  )
endif()
