set(MODULE PWGLFresonancesOutTasks)

set(SRCS
AliRsnTaskNSigma.cxx
)

set(INCLUDE_DIRS_DEP ${CMAKE_SOURCE_DIR}/PWGLFresonancesOutBase)

add_module(${MODULE} "${SRCS}" "${INCLUDE_DIRS_DEP}")
