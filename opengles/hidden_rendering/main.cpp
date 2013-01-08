/*
 *  Copyright (c) 2013 Evgeny Proydakov <lord.tiran@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#include <QImage>

#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#include <benchmark/benchmark.h>

struct UserData
{
    GLuint programObject;

};

struct ESContext
{
    void*       userData;

    GLint       width;
    GLint       height;

    EGLNativeWindowType  hWnd;

    EGLDisplay  eglDisplay;
    EGLContext  eglContext;
    EGLSurface  eglSurface;
};

void esInitContext ( ESContext *esContext )
{
    if ( esContext != NULL )
    {
        memset( esContext, 0, sizeof( ESContext) );
    }
}

EGLBoolean CreateEGLContext ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
                              EGLContext* eglContext, EGLSurface* eglSurface,
                              EGLint attribList[])
{
    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
    EGLint contextAttribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    // Get Display
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if ( display == EGL_NO_DISPLAY )
    {
        return EGL_FALSE;
    }

    // Initialize EGL
    if ( !eglInitialize(display, &majorVersion, &minorVersion) )
    {
        return EGL_FALSE;
    }

    // Get configs
    if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
    {
        return EGL_FALSE;
    }

    // Choose config
    if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
    {
        return EGL_FALSE;
    }

    // Create a surface
    //surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
    surface = eglCreatePbufferSurface(display, config, NULL);
    if ( surface == EGL_NO_SURFACE )
    {
        return EGL_FALSE;
    }

    // Create a GL context
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
    if ( context == EGL_NO_CONTEXT )
    {
        return EGL_FALSE;
    }

    // Make the context current
    if ( !eglMakeCurrent(display, surface, surface, context) )
    {
        return EGL_FALSE;
    }

    *eglDisplay = display;
    *eglSurface = surface;
    *eglContext = context;

    return EGL_TRUE;
}

GLboolean esCreateWindow ( ESContext *esContext, GLint width, GLint height)
{
    EGLint attribList[] =
    {
        EGL_SURFACE_TYPE,    EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        //EGL_LARGEST_PBUFFER, EGL_TRUE,
        EGL_RED_SIZE,        8,
        EGL_GREEN_SIZE,      8,
        EGL_BLUE_SIZE,       8,
        EGL_NONE
    };

    if ( esContext == NULL )
    {
        return GL_FALSE;
    }

    esContext->width = width;
    esContext->height = height;

    if ( !CreateEGLContext ( esContext->hWnd,
                             &esContext->eglDisplay,
                             &esContext->eglContext,
                             &esContext->eglSurface,
                             attribList) )
    {
        return GL_FALSE;
    }


    return GL_TRUE;
}

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint load_shader ( GLenum type, const char *shaderSrc )
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader ( type );

    if ( shader == 0 )
    {
        return 0;
    }

    // Load the shader source
    glShaderSource ( shader, 1, &shaderSrc, NULL );

    // Compile the shader
    glCompileShader ( shader );

    // Check the compile status
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

    if ( !compiled )
    {
        GLint infoLen = 0;

        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

        if ( infoLen > 1 )
        {
            char* infoLog = (char*) malloc (sizeof(char) * infoLen );

            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
            printf ( "Error compiling shader:\n%s\n", infoLog );

            free ( infoLog );
        }

        glDeleteShader ( shader );
        return 0;
    }

    return shader;

}

///
// Initialize the shader and program object
//
int init ( ESContext *esContext )
{
    esContext->userData = (UserData*) malloc(sizeof(UserData));

    UserData *userData = (UserData*) esContext->userData;
    GLchar vShaderStr[] =
            "attribute vec4 vPosition;    \n"
            "void main()                  \n"
            "{                            \n"
            "   gl_Position = vPosition;  \n"
            "}                            \n";

    GLchar fShaderStr[] =
            "precision mediump float;\n"\
            "void main()                                  \n"
            "{                                            \n"
            "  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
            "}                                            \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = load_shader ( GL_VERTEX_SHADER, vShaderStr );
    fragmentShader = load_shader ( GL_FRAGMENT_SHADER, fShaderStr );

    // Create the program object
    programObject = glCreateProgram ( );

    if ( programObject == 0 )
    {
        return 0;
    }

    glAttachShader ( programObject, vertexShader );
    glAttachShader ( programObject, fragmentShader );

    // Bind vPosition to attribute 0
    glBindAttribLocation ( programObject, 0, "vPosition" );

    // Link the program
    glLinkProgram ( programObject );

    // Check the link status
    glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

    if ( !linked )
    {
        GLint infoLen = 0;

        glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

        if ( infoLen > 1 )
        {
            char* infoLog = (char*) malloc (sizeof(char) * infoLen );

            glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
            printf ( "Error linking program:\n%s\n", infoLog );

            free ( infoLog );
        }

        glDeleteProgram ( programObject );
        return GL_FALSE;
    }

    // Store the program object
    userData->programObject = programObject;

    glClearColor ( 0.0f, 1.0f, 0.0f, 0.0f );
    return GL_TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void draw ( ESContext *esContext )
{
    UserData *userData = (UserData*) esContext->userData;
    GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f,
                             -0.5f, -0.5f, 0.0f,
                             0.5f, -0.5f, 0.0f
                          };

    // Set the viewport
    glViewport ( 0, 0, esContext->width, esContext->height );

    // Clear the color buffer
    glClear ( GL_COLOR_BUFFER_BIT );

    // Use the program object
    glUseProgram ( userData->programObject );

    // Load the vertex data
    glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
    glEnableVertexAttribArray ( 0 );

    glDrawArrays ( GL_TRIANGLES, 0, 3 );
}

int main ( int argc, char *argv[] )
{
    (void)(argc);
    (void)(argv);

    ESContext esContext;
    UserData  userData;

    esInitContext ( &esContext );
    esContext.userData = &userData;

    int width = 512;
    int height = 512;

    esCreateWindow ( &esContext, width, height );

    if ( !init ( &esContext ) )
    {
        return 0;
    }

    GLubyte* pixels = new GLubyte[width * height * 3];

    benchmark::clock render_benchmark(benchmark::clock::microseconds);
    render_benchmark.start();

    size_t iter = 100;
    for(size_t i = 0; i < iter; i++)
    {
        draw(&esContext);
        glClear(GL_COLOR_BUFFER_BIT);

        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, (void*) pixels);

        QImage image( (unsigned char*) pixels, width, height, QImage::Format_RGB888 );
        image.save("gles_render.bmp");
    }


    render_benchmark.stop();

    std::cout << "cycle time: " << render_benchmark.get_last_interval() / iter
              << " " << render_benchmark.get_unit_as_string() << std::endl;

    delete [] pixels;
}
