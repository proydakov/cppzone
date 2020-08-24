#==============================================================================
# Copyright (c) 2017 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

MESSAGE(STATUS "BUILD  TEMPLATE\n")

SET(GLOBAL_PROJECT_FOLDER "template")

ADD_SUBDIRECTORY(caller)
ADD_SUBDIRECTORY(delegate)
ADD_SUBDIRECTORY(fib)
ADD_SUBDIRECTORY(parser)
ADD_SUBDIRECTORY(variadic)

MESSAGE("")
