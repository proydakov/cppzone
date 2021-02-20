#pragma once

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
    #   include <OpenGLES/ES2/glext.h>
    #elif TARGET_OS_IPHONE
    #   include <OpenGLES/ES2/glext.h>
    #elif TARGET_OS_MAC
    #   include <OpenGL/gl3.h>
    #else
    #   error "Unknown Apple platform"
    #endif
#endif

#ifdef __linux__
    #include <GLES2/gl2.h>
#endif
