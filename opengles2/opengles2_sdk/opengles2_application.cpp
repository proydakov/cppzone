#include "opengles2_sdk/opengles2_application.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <execinfo.h>

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

std::string opengles2_application::load_resource(std::string const& folder, std::string const& name)
{
    std::ifstream ifs(folder + "/" + name);
    std::string content( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ) );
    return content;
}

//
//    http://www.opennet.ru/docs/formats/targa.pdf
//
//    Loads a 24-bit TGA image from a file. This is probably the simplest TGA loader ever.
//    Does not support loading of compressed TGAs.
//

std::optional<std::vector<std::byte>> opengles2_application::load_tga(std::string const& fpath, int& width, int& height, GLint& internal, GLenum& format)
{
    char tgaheader[12];
    char attributes[6];

    std::ifstream input(fpath, std::ios::binary);
    if (!input.read(tgaheader, sizeof(tgaheader)) || !input.read(attributes, sizeof(attributes)))
    {
        std::cerr << "can't read TGA header & attributes" << std::endl;
        return std::nullopt;
    }

    auto const channels = static_cast<unsigned>(attributes[4]) / 8;
    if (channels != 3 && channels != 4)
    {
        std::cerr << "unsupported channels count in TGA: " << channels << std::endl;
        return std::nullopt;
    }

    width = static_cast<int>(attributes[1]) * 256 + static_cast<int>(attributes[0]);
    height = static_cast<int>(attributes[3]) * 256 + (attributes[2]);
    unsigned imagesize = channels * static_cast<unsigned>(width * height);

    std::vector<std::byte> buffer(imagesize);
    if(input.read(reinterpret_cast<char*>(buffer.data()), imagesize))
    {
        std::cout << "loaded: " << fpath << " width: " << width << " height: " << height << " bytes: " << imagesize << std::endl;
        if (channels == 3)
        {
            internal = GL_RGB;
            format = GL_BGR;
        }
        else if (channels == 4)
        {
            internal = GL_RGBA;
            format = GL_BGRA;
        }
        return buffer;
    }
    else
    {
        std::cerr << "can't read TGA data" << std::endl;
        return std::nullopt;
    }
}

std::optional<std::vector<std::byte>> opengles2_application::load_tga(const std::string& folder, std::string const& name, int& width, int& height, GLint& internal, GLenum& format)
{
    auto const fileName(folder + "/" + name);

    return opengles2_application::load_tga(fileName, width, height, internal, format);
}

opengles2_application::opengles2_application(int, char* argv[], size_t width, size_t height) :
    m_width(width),
    m_height(height),
    m_flags(SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL),
    m_fullscreen(false),
    m_fs_functor(SDLK_F11, [this](){
        m_fullscreen = !m_fullscreen;
        std::uint32_t const new_flags = m_fullscreen ? (m_flags | SDL_WINDOW_FULLSCREEN_DESKTOP) : m_flags;
        SDL_SetWindowFullscreen(m_window, new_flags);
    })
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Unable to init SDL, error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);

    m_window = SDL_CreateWindow(extract_app_name(argv[0]).c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, int(width), int(height), m_flags);
    if(nullptr == m_window)
    {
        std::cerr << "invalid window" << std::endl;
        exit(EXIT_FAILURE);
    }
    m_context = SDL_GL_CreateContext(m_window);
}

opengles2_application::~opengles2_application()
{
}

void opengles2_application::info(std::ostream&)
{
}

void opengles2_application::on_event(SDL_Event const&)
{
}

void opengles2_application::panic()
{
    void *array[32];

    // get void*'s for all entries on the stack
    int size = backtrace(array, std::size(array));

    // print out all the frames to stderr
    fprintf(stderr, "panic:\n");
    backtrace_symbols_fd(array, size, STDERR_FILENO);

    ::abort();
}

void opengles2_application::default_info()
{
    std::clog << "Press F11 for fullscreen.\n";
    std::clog << "Press ESC for exit.\n";
    std::clog << "\n";
}

int opengles2_application::run()
{
    default_info();
    info(std::clog);
    std::clog << std::endl;

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
                    on_event(e);
                    break;
                }
                break;

            case SDL_WINDOWEVENT:
                switch (e.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    m_width = size_t(e.window.data1);
                    m_height = size_t(e.window.data2);
                    resize(m_width, m_height);
                }
                    break;
                }
                break;

            case SDL_QUIT:
                running = false;
                break;

            default:
                on_event(e);
                break;
            }
        }

        auto now = std::chrono::high_resolution_clock::now();
        auto const delta = std::chrono::duration_cast<std::chrono::microseconds>(now - m_last);

        if (delta < std::chrono::milliseconds(100))
        {
            update(delta);
            draw();
        }

        m_last = now;
        SDL_GL_SwapWindow(m_window);
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

keyboard_press_guard::keyboard_press_guard(SDL_Keycode code, std::function<void()>&& lambda)
    : m_code(code)
    , m_lock(false)
    , m_lambda(std::move(lambda))
{
}

void keyboard_press_guard::operator()(SDL_Event const& Event)
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
