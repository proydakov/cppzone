###############################################################################
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################

MACRO(COMPILER_SET_MAX_WARNING_LEVEL)

IF(MSVC)
    STRING(REGEX REPLACE /W[0-4] /W4 CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
ELSE()
    SET(CMAKE_C_FLAGS  "-Wall")
    SET(CMAKE_CXX_FLAGS "-Wall")
ENDIF(MSVC)

ENDMACRO(COMPILER_SET_MAX_WARNING_LEVEL)

MACRO(COMPILER_MSVC_SET_STATIC_RUNTIME)

IF(NOT MSVC)
    MESSAGE(FATAL_ERROR "Invalid macro call MSVC_SET_STATIC_RUNTIME. The compiler does not meet the requirements.")
ENDIF(NOT MSVC)
    
#We statically link to reduce dependancies
FOREACH(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    IF(${flag_var} MATCHES "/MD")
        STRING(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
    ENDIF(${flag_var} MATCHES "/MD")
    IF(${flag_var} MATCHES "/MDd")
        STRING(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
    ENDIF(${flag_var} MATCHES "/MDd")
ENDFOREACH(flag_var)

ENDMACRO(COMPILER_MSVC_SET_STATIC_RUNTIME)