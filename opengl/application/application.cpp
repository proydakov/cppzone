#include "application.h"

#include <chrono>
#include <iostream>

namespace
{
    std::string extract_app_name(char* argv)
    {
        std::string fullname(argv);
        std::size_t index = fullname.find_last_of('/');
        index = (std::string::npos == index) ? 0 : index + 1;
        return fullname.substr(index);
    }
}

application::application(int argc, char* argv[], size_t width, size_t height) :
    m_flags(SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL),
    m_width(width),
    m_height(height),
    m_fullscreen(false),
    m_fs_functor(SDLK_F11, [this](){
        m_fullscreen = !m_fullscreen;
        std::int32_t const new_flags = m_fullscreen ? (m_flags | SDL_WINDOW_FULLSCREEN_DESKTOP) : m_flags;
        SDL_SetWindowFullscreen(m_window, new_flags);
    })
{
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ){
        std::cerr << "Unable to init SDL, error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);

    m_window = SDL_CreateWindow(extract_app_name(argv[0]).c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, m_flags);
    if(nullptr == m_window)
    {
        std::cerr << "invalid window" << std::endl;
        exit(EXIT_FAILURE);
    }
    m_context = SDL_GL_CreateContext(m_window);
}

void application::info()
{
}

void application::keyboard(SDL_Event const& Event)
{
    (void)(Event);
}

void application::default_info()
{
    std::cout << "Press F11 for fullscreen.\n";
    std::cout << "Press ESC for exit.\n";
    std::cout << "\n";
}

int application::run()
{
    default_info();
    info();
    std::cout << std::endl;

    resize(m_width, m_height);
    init();

    auto m_last = std::chrono::high_resolution_clock::now();

    bool running = true;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                switch (e.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;

                case SDLK_F11:
                    m_fs_functor(e);
                    break;

                default:
                    keyboard(e);
                    break;
                }
                break;

            case SDL_WINDOWEVENT:
                    switch (e.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                    {
                        m_width = e.window.data1;
                        m_height = e.window.data2;
                        resize(m_width, m_height);
                    }
                        break;
                    }
                break;

            case SDL_QUIT:
                running = false;
                break;

            default:
                break;
            }
        }

        auto now = std::chrono::high_resolution_clock::now();
        auto const delta = std::chrono::duration_cast<std::chrono::microseconds>(now - m_last);

        update(delta);
        draw();

        m_last = now;
        SDL_GL_SwapWindow(m_window);
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////


keyboard_press_guard::keyboard_press_guard(SDL_Keycode code, std::function<void()>&& lambda) :
    m_code(code),
    m_lock(false),
    m_lambda(std::move(lambda))
{
}

void keyboard_press_guard::operator ()(SDL_Event const& Event)
{
    switch (Event.type) {
    case SDL_KEYDOWN:
        if(m_code == Event.key.keysym.sym && m_lock == false)
        {
            m_lock = true;
            m_lambda();
        }
        break;

    case SDL_KEYUP:
        if(m_code == Event.key.keysym.sym && m_lock == true)
        {
            m_lock = false;
        }
        break;
    }
}
