cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

macro(KDevelopSettings)

  FIND_PROGRAM(KDEVELOP_EXECUTABLE kdevelop)
  if(KDEVELOP_EXECUTABLE)
    message(STATUS "Generating KDevelop settings ...")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/kdev_include_paths.tmp ${CMAKE_SOURCE_DIR}/.kdev_include_paths)
  endif(KDEVELOP_EXECUTABLE)

endmacro(KDevelopSettings)
