/*
 *  Copyright (c) 2012-2013 Evgeny Proydakov <lord.tiran@gmail.com>
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
#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <common/iglut.h>

#include <maze.h>

struct application_t
{
    int window_width;
    int window_height;

    int maze_width;
    int maze_height;

    bool animate;

    boost::shared_ptr<maze> sp_maze;
    maze::maze_type type;
};

application_t g_app;

const std::string COMMENT = "Press 'g' or 'G' for generate new maze.\nPress '1', '2', '3' for select maze type;\nPress Esc for exit...";

void info()
{
    std::cout << COMMENT << std::endl;
}

void new_maze()
{
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    g_app.sp_maze.reset();
    if(g_app.type == maze::maze_type::fixed) {
        g_app.sp_maze = fixed();
    }
    else if(g_app.type == maze::maze_type::empty) {
        g_app.sp_maze = empty_maze(g_app.maze_width, g_app.maze_height);
    }
    else if(g_app.type == maze::maze_type::random) {
        g_app.sp_maze = random_maze(g_app.maze_width, g_app.maze_height);
    }
    g_app.sp_maze->solve();
}

void set_heuristic(maze::heuristic_type h)
{
    g_app.sp_maze->set_heuristic(h);
    g_app.sp_maze->solve();
}

void animate()
{
    g_app.animate = ! g_app.animate;
}

void init()
{
    glShadeModel(GL_FLAT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    vertices_size_type nx = g_app.sp_maze->length(0);
    vertices_size_type ny = g_app.sp_maze->length(1);

    const double size = std::min(g_app.window_width / nx, g_app.window_height / ny);

    // barrier
    {
        glColor3d(0.0, 0.5, 1);
        auto way = g_app.sp_maze->get_barriers();
        for(const auto& el : way) {
            auto property = g_app.sp_maze->get_vertex_property(el);
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
        auto vector = g_app.sp_maze->get_heuristic_tested();
        for(const auto& el : vector) {
            auto property = g_app.sp_maze->get_vertex_property(el);
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
        auto solution = g_app.sp_maze->get_solution();
        for(const auto& el : solution) {
            auto property = g_app.sp_maze->get_vertex_property(el);
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
        vertex_descriptor s = g_app.sp_maze->source_vertex();

        auto property = g_app.sp_maze->get_vertex_property(s);
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
        vertex_descriptor g = g_app.sp_maze->goal_vertex();

        auto property = g_app.sp_maze->get_vertex_property(g);
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

        double width = ny * size;

        for (vertices_size_type y = 0; y < ny + 1; y++) {
            double h = y * size;
            glBegin(GL_LINES);
            {
                glVertex3d(  0.0, h, 0.0);
                glVertex3d(width, h, 0.0);
            }
            glEnd();
        }

        double height = nx * size;

        for (vertices_size_type x = 0; x < nx + 1; x++) {
            double w = x * size;
            glBegin(GL_LINES);
            {
                glVertex3d(w,    0.0, 0.0);
                glVertex3d(w, height, 0.0);
            }
            glEnd();
        }
    }
    
    glFlush();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    switch(key) {
    case 27:
        exit(0);
        break;

    case '1':
        g_app.type = maze::maze_type::fixed;
        new_maze();
        break;

    case '2':
        g_app.type = maze::maze_type::empty;
        new_maze();
        break;

    case '3':
        g_app.type = maze::maze_type::random;
        new_maze();
        break;

    case 'G':
    case 'g':
        new_maze();
        break;

    case 'E':
    case 'e':
        set_heuristic(maze::heuristic_type::euclidean);
        break;

    case 'S':
    case 's':
        set_heuristic(maze::heuristic_type::euclidean_squared);
        break;

    case 'M':
    case 'm':
        set_heuristic(maze::heuristic_type::manhattan);
        break;

    case 'D':
    case 'd':
        set_heuristic(maze::heuristic_type::diagonal);
        break;

//    case 'A':
//    case 'a':
//        animate();
//        break;

    default:
        break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h)
{
    g_app.window_width = w;
    g_app.window_height = h;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
    info();

    std::size_t size = 1000;

    std::size_t x = size;
    std::size_t y = size;

    if (argc == 3) {
        x = boost::lexical_cast<std::size_t>(argv[1]);
        y = boost::lexical_cast<std::size_t>(argv[2]);
    }

    g_app.window_width = 0;
    g_app.window_height = 0;
    g_app.maze_width = x;
    g_app.maze_height = y;
    g_app.animate = false;
    g_app.type = maze::maze_type::fixed;

    random_generator.seed(std::time(0));

    new_maze();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
