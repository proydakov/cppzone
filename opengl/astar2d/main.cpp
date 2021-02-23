/*
 *  Copyright (c) 2012-2020 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <ctime>
#include <memory>
#include <iostream>
#include <algorithm>

#include <maze.h>

#include <opengl_sdk/application.h>

class tcapplication : public application
{
public:
    tcapplication(int argc, char* argv[], std::size_t w, std::size_t h);

    void init() override;
    void resize(std::size_t w, std::size_t h) override;
    void update(std::chrono::microseconds delta) override;
    void draw() override;

    void info(std::ostream&) override;
    void on_event(SDL_Event const& e) override;

    void new_maze();
    void set_heuristic(maze::heuristic_type h);

public:
    size_t m_window_width;
    size_t m_window_height;

    size_t m_maze_width;
    size_t m_maze_height;

    std::shared_ptr<maze> m_sp_maze;
    maze::maze_type m_type;

private:
    keyboard_press_guard m_functor_fixed;
    keyboard_press_guard m_functor_empty;
    keyboard_press_guard m_functor_random;
    keyboard_press_guard m_functor_generate;
    keyboard_press_guard m_functor_euclidean;
    keyboard_press_guard m_functor_euclidean_squared;
    keyboard_press_guard m_functor_manhattan;
    keyboard_press_guard m_functor_diagonal;
};

tcapplication::tcapplication(int argc, char* argv[], std::size_t w, std::size_t h) :
    application(argc, argv, w, h),
    m_window_width(w),
    m_window_height(h),
    m_maze_width(100),
    m_maze_height(100),
    m_functor_fixed(SDLK_1, [this](){
        m_type = maze::maze_type::fixed;
        new_maze();
    }),
    m_functor_empty(SDLK_2, [this](){
        m_type = maze::maze_type::empty;
        new_maze();
    }),
    m_functor_random(SDLK_3, [this](){
        m_type = maze::maze_type::random;
        new_maze();
    }),
    m_functor_generate(SDLK_g, [this](){
        new_maze();
    }),
    m_functor_euclidean(SDLK_e, [this](){
        set_heuristic(maze::heuristic_type::euclidean);
    }),
    m_functor_euclidean_squared(SDLK_s, [this](){
        set_heuristic(maze::heuristic_type::euclidean_squared);
    }),
    m_functor_manhattan(SDLK_m, [this](){
        set_heuristic(maze::heuristic_type::manhattan);
    }),
    m_functor_diagonal(SDLK_d, [this](){
        set_heuristic(maze::heuristic_type::diagonal);
    })
{
}

void tcapplication::init()
{
    glShadeModel(GL_FLAT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void tcapplication::resize(std::size_t w, std::size_t h)
{
    m_window_width = w;
    m_window_height = h;

    glViewport(0, 0, GLsizei(w), GLsizei(h));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, GLdouble(w), GLdouble(h), 0);
    glMatrixMode(GL_MODELVIEW);
}

void tcapplication::update(std::chrono::microseconds)
{
}

void tcapplication::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    vertices_size_type const nx = m_sp_maze->length(0);
    vertices_size_type const ny = m_sp_maze->length(1);

    auto const size = std::fmin(GLdouble(m_window_width) / GLdouble(nx), GLdouble(m_window_height) / GLdouble(ny));

    // barrier
    {
        glColor3d(0.0, 0.5, 1);
        auto way = m_sp_maze->get_barriers();
        for(const auto& el : way)
        {
            auto property = m_sp_maze->get_vertex_property(el);
            int x = property.x;
            int y = property.y;

            glLoadIdentity();
            glTranslated(size * x, size * y, 0);

            glBegin(GL_POLYGON);
            {
                glVertex3d( 0.0,  0.0, 0.0);
                glVertex3d( 0.0, size, 0.0);
                glVertex3d(size, size, 0.0);
                glVertex3d(size,  0.0, 0.0);
            }
            glEnd();
        }
    }

    // way
    {
        glColor3d(0.8, 0.75, 0.75);
        auto vector = m_sp_maze->get_heuristic_tested();
        for(const auto& el : vector)
        {
            auto property = m_sp_maze->get_vertex_property(el);
            int x = property.x;
            int y = property.y;

            glLoadIdentity();
            glTranslated(size * x, size * y, 0);

            glBegin(GL_POLYGON);
            {
                glVertex3d( 0.0,  0.0, 0.0);
                glVertex3d( 0.0, size, 0.0);
                glVertex3d(size, size, 0.0);
                glVertex3d(size,  0.0, 0.0);
            }
            glEnd();
        }
    }

    // solution
    {
        glColor3d(1, 0.5, 0);
        auto solution = m_sp_maze->get_solution();
        for(const auto& el : solution)
        {
            auto property = m_sp_maze->get_vertex_property(el);
            int x = property.x;
            int y = property.y;

            glLoadIdentity();
            glTranslated(size * x, size * y, 0);

            glBegin(GL_POLYGON);
            {
                glVertex3d( 0.0,  0.0, 0.0);
                glVertex3d( 0.0, size, 0.0);
                glVertex3d(size, size, 0.0);
                glVertex3d(size,  0.0, 0.0);
            }
            glEnd();
        }
    }

    // start
    {
        vertex_descriptor s = m_sp_maze->source_vertex();

        auto property = m_sp_maze->get_vertex_property(s);
        int x = property.x;
        int y = property.y;

        glLoadIdentity();
        glTranslated(size * x, size * y, 0);

        glColor3d(0, 1, 0);

        glBegin(GL_POLYGON);
        {
            glVertex3d( 0.0,  0.0, 0.0);
            glVertex3d( 0.0, size, 0.0);
            glVertex3d(size, size, 0.0);
            glVertex3d(size,  0.0, 0.0);
        }
        glEnd();
    }

    // end
    {
        vertex_descriptor g = m_sp_maze->goal_vertex();

        auto property = m_sp_maze->get_vertex_property(g);
        int x = property.x;
        int y = property.y;

        glLoadIdentity();
        glTranslated(size * x, size * y, 0);

        glColor3d(1, 0, 0);

        glBegin(GL_POLYGON);
        {
            glVertex3d( 0.0,  0.0, 0.0);
            glVertex3d( 0.0, size, 0.0);
            glVertex3d(size, size, 0.0);
            glVertex3d(size,  0.0, 0.0);
        }
        glEnd();
    }

    // grid
    {
        glLoadIdentity();
        glColor3d(0, 0, 1);

        double width = double(ny) * double(size);

        for (vertices_size_type y = 0; y < ny + 1; y++)
        {
            double h = double(y) * double(size);
            glBegin(GL_LINES);
            {
                glVertex3d(  0.0, h, 0.0);
                glVertex3d(width, h, 0.0);
            }
            glEnd();
        }

        double height = double(nx) * double(size);

        for (vertices_size_type x = 0; x < nx + 1; x++)
        {
            double w = double(x) * double(size);
            glBegin(GL_LINES);
            {
                glVertex3d(w,    0.0, 0.0);
                glVertex3d(w, height, 0.0);
            }
            glEnd();
        }
    }
}

void tcapplication::info(std::ostream& os)
{
    os << "Press 'g' or 'G' for generate new maze.\nPress '1', '2', '3' for select maze type\n";
}

void tcapplication::on_event(SDL_Event const& e)
{
    switch (e.type)
    {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            switch (e.key.keysym.sym)
            {
            case SDLK_1:
                m_functor_fixed(e);
                break;

            case SDLK_2:
                m_functor_empty(e);
                break;

            case SDLK_3:
                m_functor_random(e);
                break;

            case SDLK_g:
                m_functor_generate(e);
                break;

            case SDLK_e:
                m_functor_euclidean(e);
                break;

            case SDLK_s:
                m_functor_euclidean_squared(e);
                break;

            case SDLK_m:
                m_functor_manhattan(e);
                break;

            case SDLK_d:
                m_functor_diagonal(e);
                break;

            default:
                break;
            }
            break;
    }
}

void tcapplication::new_maze()
{
    m_sp_maze.reset();

    switch (m_type) {
    case maze::maze_type::fixed:
        m_sp_maze = fixed();
        break;

    case maze::maze_type::empty:
        m_sp_maze = empty_maze(m_maze_width, m_maze_height);
        break;

    case maze::maze_type::random:
        m_sp_maze = random_maze(m_maze_width, m_maze_height);
        break;

    default:
        break;
    }

    m_sp_maze->solve();
}

void tcapplication::set_heuristic(maze::heuristic_type h)
{
    m_sp_maze->set_heuristic(h);
    m_sp_maze->solve();
}

int main(int argc, char* argv[])
{
    constexpr std::size_t size = 75;

    std::size_t x = size;
    std::size_t y = size;

    if (argc == 3)
    {
        x = std::stoul(argv[1]);
        y = std::stoul(argv[2]);
    }

    tcapplication app(argc, argv, 600, 600);

    app.m_window_width = 0;
    app.m_window_height = 0;
    app.m_maze_width = x;
    app.m_maze_height = y;
    app.m_type = maze::maze_type::fixed;
    app.new_maze();

    return app.run();
}
