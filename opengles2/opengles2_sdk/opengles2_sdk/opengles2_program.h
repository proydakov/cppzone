#pragma once

#include <set>
#include <string>
#include <vector>
#include <utility>
#include <optional>

#include "opengles2.h"

class opengles2_program final
{
public:
    opengles2_program();
    ~opengles2_program();

    bool load(const char * const vertex_shader,
              const char * const fragment_shader,
              const std::set<std::string>& attributes,
              const std::set<std::string>& uniforms);
    void unload();

    GLuint get_id() const;
    std::optional<GLuint> get_attribute_location(const std::string& attribute) const;
    std::optional<GLint>  get_uniform_location(const std::string& uniform) const;

private:
    GLuint load_shader(GLenum type, const char* shaderSrc);

private:
    GLuint m_program;

    std::vector<std::pair<std::string, int>> m_attributes;
    std::vector<std::pair<std::string, int>> m_uniforms;
};
