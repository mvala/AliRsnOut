function(add_module MODULE SRC INCLUDE_DIRS_DEP) 

  set(INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/${MODULE}
    ${ROOT_INCLUDE_DIR}
    )
  include_directories( ${INCLUDE_DIRS_DEP} ${INCLUDE_DIRECTORIES})
  
  set(LINK_DIRECTORIES ${ROOT_LIBRARY_DIR})
  
  link_directories( ${LINK_DIRECTORIES})
  
  String(REPLACE ".cxx" ".h" HDRS "${SRCS}")
  ROOT_GENERATE_DICTIONARY( G__${MODULE} ${HDRS} LINKDEF ${MODULE}LinkDef.h)
  list(APPEND SRCS G__${MODULE}.cxx)
  
  add_library(${MODULE} SHARED ${SRCS} ${HDRS})
  target_link_libraries(${MODULE} ${ROOT_LIBRARIES})
  
  install(TARGETS ${MODULE}
    RUNTIME DESTINATION bin COMPONENT RUNTIME
    LIBRARY DESTINATION lib COMPONENT RUNTIME
    ARCHIVE DESTINATION lib COMPONENT DEVELOPMENT)
  
  install(FILES ${HDRS} DESTINATION include)
  
  #add_par(${MODULE})
  
endfunction()

function(add_par PACKAGE)
  if(PACKAGE)
  message(STATUS "Generating ${PACKAGE}")
  add_custom_target(${PACKAGE}.par
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/pars
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} tar --exclude=.svn -czhf ${CMAKE_SOURCE_DIR}/pars/${PACKAGE}.par ${PACKAGE}
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red --bold "${PACKAGE}.par has been created in ${CMAKE_SOURCE_DIR}/pars"
#    DEPENDS ${PARSRCS}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
  
  add_dependencies(par-all ${PACKAGE}.par)
  
  endif(PACKAGE)
endfunction(add_par PACKAGE)