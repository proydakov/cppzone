#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# CMake for  SYNTAX  PROJECT
#==============================================================================

MESSAGE(STATUS "BUILD  SYNTAX\n")

SET(GLOBAL_PROJECT_FOLDER "syntax")

ADD_SUBDIRECTORY(address)
ADD_SUBDIRECTORY(align)
ADD_SUBDIRECTORY(alignof)
ADD_SUBDIRECTORY(bind)
ADD_SUBDIRECTORY(bits)
ADD_SUBDIRECTORY(cycle)
ADD_SUBDIRECTORY(constructors)
ADD_SUBDIRECTORY(typeinfo)
ADD_SUBDIRECTORY(inheritance_2)
ADD_SUBDIRECTORY(macro)
ADD_SUBDIRECTORY(move)
ADD_SUBDIRECTORY(placement_new)
ADD_SUBDIRECTORY(exception_basic)
ADD_SUBDIRECTORY(exception_smart)
ADD_SUBDIRECTORY(exception_in_constructor)
ADD_SUBDIRECTORY(operator_brackets)
ADD_SUBDIRECTORY(operator_cast_type)
ADD_SUBDIRECTORY(sizeof)
ADD_SUBDIRECTORY(numeric_limits)
ADD_SUBDIRECTORY(return_const_ref)
ADD_SUBDIRECTORY(template_fun)
ADD_SUBDIRECTORY(template_explicit_specialization_function)
ADD_SUBDIRECTORY(virtual)
ADD_SUBDIRECTORY(unique_ptr)
ADD_SUBDIRECTORY(union)

MESSAGE("")
