#==============================================================================
# Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

FIND_PACKAGE(SDL2)

SET(HEADER_FILES
    texture_loader.h
    )

SET(SOURCE_FILES
    texture_loader.cpp
    )

ADD_LIBRARY(texture_loader STATIC ${HEADER_FILES} ${SOURCE_FILES})
SET_PROPERTY(TARGET texture_loader PROPERTY FOLDER ${GLOBAL_PROJECT_FOLDER})
TARGET_LINK_LIBRARIES(texture_loader ${SDL2_LIBRARY})
TARGET_INCLUDE_DIRECTORIES(texture_loader SYSTEM PUBLIC ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

if(MSVC)
    TARGET_COMPILE_OPTIONS(texture_loader PRIVATE "/wd26812")
endif()

SOURCE_GROUP(source FILES ${HEADER_FILES} ${SOURCE_FILES})
