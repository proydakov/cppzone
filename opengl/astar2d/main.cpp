/*
 *  Copyright (c) 2012 Evgeny Proydakov <lord.tiran@gmail.com>
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

#include <GL/glut.h>

#include <maze.h>

struct application_t
{
    int window_width;
    int window_height;

    int maze_width;
    int maze_height;

    maze::maze_type type;

    boost::shared_ptr<maze> p_maze;
};

application_t g_app;

const std::string COMMENT = "Press 'g' or 'G' for generate new maze.\nPress '1', '2', '3' for select maze type;\nPress Esc for exit...";

void info()
{
    std::cout << COMMENT << std::endl;
}

void new_maze()
{
    g_app.p_maze.reset();
    if(g_app.type == maze::fixed) {
        g_app.p_maze = boost::shared_ptr<maze>(new maze(fixed()));
    }
    else if(g_app.type == maze::empty) {
        g_app.p_maze = boost::shared_ptr<maze>(new maze(empty_maze(g_app.maze_width, g_app.maze_height)));
    }
    else if(g_app.type == maze::random) {
        g_app.p_maze = boost::shared_ptr<maze>(new maze(random_maze(g_app.maze_width, g_app.maze_height)));
    }
    g_app.p_maze->solve();
}

void init()
{
    glShadeModel(GL_FLAT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    vertices_size_type nx = g_app.p_maze->length(0);
    vertices_size_type ny = g_app.p_maze->length(1);

    const double size = std::min(g_app.window_width / nx, g_app.window_height / ny);
    double obj_half_size = 0;

    // maze
    for (vertices_size_type y = 0; y < ny; y++) {
        for (vertices_size_type x = 0; x < nx; x++) {

            obj_half_size = size / 2;

            glLoadIdentity();
            glTranslated(size * x + size / 2, size * y + size / 2, 0);

            vertex_descriptor u = {{x, y}};
            if (g_app.p_maze->solution_contains(u)) {
                glColor3d(0.7, 0, 0);

                obj_half_size /= 2;
            }
            else if (g_app.p_maze->has_barrier(u)) {
                glColor3d(0.0, 0.5, 1);
            }
            else if (g_app.p_maze->way_contains(u)) {
                glColor3d(0.4, 0.7, 0.5);

                obj_half_size /= 2;
            }
            else {
                glColor3d(0, 0, 0);
            }
            glBegin(GL_POLYGON);
            {
                glVertex3d(-obj_half_size, -obj_half_size, 0.0);
                glVertex3d(-obj_half_size,  obj_half_size, 0.0);
                glVertex3d( obj_half_size,  obj_half_size, 0.0);
                glVertex3d( obj_half_size, -obj_half_size, 0.0);
            }
            glEnd();
        }
    }

    // start
    {
        vertex_descriptor s = g_app.p_maze->source();

        int x = s[0];
        int y = s[1];

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
        vertex_descriptor g = g_app.p_maze->goal();

        int x = g[0];
        int y = g[1];

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
        g_app.type = maze::fixed;
        new_maze();
        break;

    case '2':
        g_app.type = maze::empty;
        new_maze();
        break;

    case '3':
        g_app.type = maze::random;
        new_maze();
        break;

    case 'G':
    case 'g':
        new_maze();
        break;

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

    std::size_t size = 100;

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
    g_app.type = maze::fixed;

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
