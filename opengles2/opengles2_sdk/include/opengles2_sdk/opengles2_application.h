#pragma once

#include <chrono>
#include <cstdint>
#include <ostream>
#include <functional>

#include "opengles2.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

class opengles2_texture;

class keyboard_press_guard
{
public:
    keyboard_press_guard(SDL_Keycode code, std::function<void()>&& lambda);
    void operator()(SDL_Event const& Event);

private:
    SDL_Keycode const m_code;
    bool m_lock;
    std::function<void()> m_lambda;
};

class opengles2_application
{
public:
    static std::string load_resource(std::string const& folder, std::string const& name);
    static bool load_tga(opengles2_texture&, std::string const& fpath);
    static bool load_tga(opengles2_texture&, std::string const& folder, std::string const& name);

    opengles2_application(int argc, char* argv[], std::size_t width, std::size_t height);
    virtual ~opengles2_application();

    virtual void init() = 0;
    virtual void resize(std::size_t width, std::size_t height) = 0;
    virtual void update(std::chrono::microseconds delta) = 0;
    virtual void draw() = 0;

    virtual void info(std::ostream&);
    virtual void on_event(SDL_Event const&);

    int run();

    // unused
    opengles2_application(opengles2_application const&) = delete;
    opengles2_application(opengles2_application &&) = delete;
    opengles2_application& operator=(opengles2_application const&) = delete;
    opengles2_application& operator=(opengles2_application &&) = delete;

    static void panic_impl(const char* name, int line, const char* function);

protected:
    std::size_t getWidth() const noexcept
    {
        return m_width;
    }

    std::size_t getHeight() const noexcept
    {
        return m_height;
    }

private:
    void default_info();

private:
    SDL_Window*   m_window;
    SDL_GLContext m_context;

    std::size_t   m_width;
    std::size_t   m_height;
    std::uint32_t m_flags;
    bool          m_fullscreen;

    keyboard_press_guard m_fs_functor;
};

#define panic() opengles2_application::panic_impl(__FILE__, __LINE__, __PRETTY_FUNCTION__);
