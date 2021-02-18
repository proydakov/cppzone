#include <iostream>
#include <algorithm>

#include "opengles2_sdk/opengles2_program.h"

opengles2_program::opengles2_program() :
    m_program(0)
{
}

opengles2_program::~opengles2_program()
{
    unload();
}

bool opengles2_program::load(
          const char* const vertex_shader,
          const char* const fragment_shader,
          const std::set<std::string>& attributes,
          const std::set<std::string>& uniforms)
{
    unload();

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;

    // Load the vertex/fragment shaders
    vertexShader = load_shader(GL_VERTEX_SHADER, vertex_shader);
    fragmentShader = load_shader(GL_FRAGMENT_SHADER, fragment_shader);

    if (0 == vertexShader || 0 == fragmentShader) {
        return false;
    }

    // Create the program object
    programObject = glCreateProgram();

    if (programObject == 0) {
        return false;
    }

    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    // Link the program
    glLinkProgram(programObject);

    // Check the link status
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            char* infoLog = (char*) malloc (static_cast<size_t>(infoLen));
            glGetProgramInfoLog( programObject, infoLen, NULL, infoLog );
            std::cerr << "Error linking program:\n" << infoLog << std::endl;
            free(infoLog);
        }
        glDeleteProgram(programObject);
        return false;
    }

    // Store the program object
    m_program = programObject;

    for(auto const& name : attributes) {
        m_attributes.emplace_back(name, glGetAttribLocation(programObject, name.c_str()));
    }

    for(auto const& name : uniforms) {
        m_uniforms.emplace_back(name, glGetUniformLocation(programObject, name.c_str()));
    }

    return true;
}

GLuint opengles2_program::get_id() const
{
    return m_program;
}

GLuint opengles2_program::get_attribute_location(const std::string& attribute) const
{
    auto it = std::lower_bound(m_attributes.begin(), m_attributes.end(), attribute, [](const auto& pair, auto const& str){
        return pair.first < str;
    });
    GLuint location = 0;
    if(it != m_attributes.end() && it->first == attribute) {
        location = static_cast<GLuint>(it->second);
    }
    return location;
}

GLuint opengles2_program::get_uniform_location(const std::string& uniform) const
{
    auto it = std::lower_bound(m_uniforms.begin(), m_uniforms.end(), uniform, [](const auto& pair, auto const& str){
        return pair.first < str;
    });    
    GLuint location = 0;
    if(it != m_uniforms.end() && it->first == uniform) {
        location = static_cast<GLuint>(it->second);
    }
    return location;
}

void opengles2_program::unload()
{
    if (m_program) {
        glDeleteProgram(m_program);
        m_program = 0;
    }
}

GLuint opengles2_program::load_shader(GLenum type, const char *shaderSrc)
{
    GLuint shader;
    GLint compiled;

    // Create the shader object
    shader = glCreateShader(type);

    if (shader == 0) {
        return 0;
    }

    // Load the shader source
    glShaderSource(shader, 1, &shaderSrc, NULL);

    // Compile the shader
    glCompileShader(shader);

    // Check the compile status
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
        if (infoLen > 1) {
            char* infoLog = (char*) malloc(static_cast<size_t>(infoLen));
            glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
            std::cerr << "Error compiling shader:\n" << infoLog << std::endl;
            free(infoLog);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}
