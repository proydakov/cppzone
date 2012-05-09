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
#include <cmath>
#include <string>
#include <cassert>
#include <iostream>

#include <GL/glut.h>

#include <cube.h>
#include <torus.h>
#include <lines.h>
#include <chaos.h>
#include <spiral.h>
#include <sphere.h>
#include <paraboloid.h>

const std::string COMMENT = 
"Keys 'w', 's', 'd', 'a' to rotate the object.\n\
Keys '1 ', '2', '3', '4', '5', '6', '7' are used to select the type of object.\n\
Key 'k' is used to select the type of morphing.\
Press ESC for exit.";

const int CYCLE_TIME = 10;
const int ESCAPE = 27;

const int MORPHING_STEPS = 150;

int g_morphing_step = 0;
bool g_morphing = false;

enum morphing_types { point_morphing, line_morphing };

morphing_types g_morphing_types = point_morphing;

const GLfloat MAX_POINT_DELTA = 0.01f;
const GLfloat SPEED_DELTA = 0.05f;

typedef object< point3d<GLfloat>, color_rgba<GLdouble> > scene_object;

const int OBJECT_SPHERE      = 0;
const int OBJECT_TORUS       = 1;
const int OBJECT_LINES       = 2;
const int OBJECT_CUBE        = 3;
const int OBJECT_SPIRAL      = 4;
const int OBJECT_PARABOLOID  = 5;
const int OBJECT_CHAOS       = 6;
int g_object_index = OBJECT_SPHERE;

scene_object g_object_sphere;
scene_object g_object_torus;
scene_object g_object_lines;
scene_object g_object_cube;
scene_object g_object_spiral;
scene_object g_object_paraboloid;
scene_object g_object_chaos;
scene_object g_object;
scene_object g_object_last;

std::vector<scene_object> g_objects;

GLfloat g_move_delta = 0;

GLfloat g_ySpeed = 0;
GLfloat g_zSpeed = 0.2f;

GLfloat g_yRotation = 0;
GLfloat g_zRotation = 0;

void info()
{
    std::cout << COMMENT << "\n" << std::endl;
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
 
    glPointSize(1.7f);
    
    srand((unsigned)time(NULL));

    int size = 1600;

    scene_object::color_type color(0.1, 0.5, 1.0, 1.0);

    generate_sphere(g_object_sphere, color, 0.5f, size);
    generate_torus(g_object_torus, color, 0.5f, 0.1f, size);
    generate_cube(g_object_cube, color, 1.0f, size);
    generate_lines(g_object_lines, color, 0.5f, size);
    generate_spiral(g_object_spiral, color, 0.5f, size);
    generate_paraboloid(g_object_paraboloid, color, 0.5f, size);
    generate_chaos(g_object_chaos, color, 0.5f, size);
    
    g_objects.push_back(g_object_sphere);
    g_objects.push_back(g_object_torus);
    g_objects.push_back(g_object_lines);
    g_objects.push_back(g_object_cube);
    g_objects.push_back(g_object_spiral);
    g_objects.push_back(g_object_paraboloid);
    g_objects.push_back(g_object_chaos);

    g_object = g_objects[g_object_index];

    std::cout << "Number of points in the object : " << g_object.points.size() << std::endl;

    g_move_delta = 1.0f / MORPHING_STEPS;
}

void draw(scene_object& object, scene_object::color_type morphing_color)
{
    if(g_morphing_types == point_morphing)
        glBegin(GL_POINTS);
    else
        glBegin(GL_LINES);
    {
        for(size_t i = 0; i < object.points.size(); ++i) {
            scene_object::vertex data = object.points[i];

            GLfloat x = data.m_point.m_x + MAX_POINT_DELTA / (rand() % 10) - MAX_POINT_DELTA / 2;
            GLfloat y = data.m_point.m_y + MAX_POINT_DELTA / (rand() % 10) - MAX_POINT_DELTA / 2;
            GLfloat z = data.m_point.m_z + MAX_POINT_DELTA / (rand() % 10) - MAX_POINT_DELTA / 2;

            if(!g_morphing) {
                glColor4d(data.m_color.m_red, data.m_color.m_green, data.m_color.m_blue, data.m_color.m_alpha);
            }
            else {
                glColor4d(morphing_color.m_red, morphing_color.m_green, morphing_color.m_blue, morphing_color.m_alpha);
            }

            glVertex3f(x, y, z);
        }
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    glRotatef(g_yRotation, 0.0, 1.0, 0.0);
    glRotatef(g_zRotation, 0.0, 0.0, 1.0);

    scene_object::color_type morphing_color(0.0, 0.8, 1.0, 1.0);
    scene_object::color_type morphing_color_last = morphing_color;
    morphing_color_last.m_alpha = morphing_color_last.m_alpha * 7 / 10;

    draw(g_object, morphing_color);
    if(g_morphing) {
        draw(g_object_last, morphing_color_last);
    }

    glPopMatrix();

    glLoadIdentity();
    gluLookAt(1.7, 1.5, 1.5, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

    glFlush();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (GLdouble) w / (GLdouble) h, 1.0, 20.0);
}

void copy_color(scene_object& dst, const scene_object& src)
{
    assert(dst.points.size() == src.points.size());

    for(size_t i = 0; i < dst.points.size(); ++i) {
        dst.points[i].m_color = src.points[i].m_color;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    switch(key) {
        case ESCAPE:
            exit(0);
            break;

        case '1':
            g_object_index = OBJECT_SPHERE;
            copy_color(g_object, g_object_sphere);
            break;

        case '2':
            g_object_index = OBJECT_TORUS;
            copy_color(g_object, g_object_torus);
            break;

        case '3':
            g_object_index = OBJECT_LINES;
            copy_color(g_object, g_object_lines);
            break;

        case '4':
            g_object_index = OBJECT_CUBE;
            copy_color(g_object, g_object_cube);
            break;

        case '5':
            g_object_index = OBJECT_SPIRAL;
            copy_color(g_object, g_object_spiral);
            break;

        case '6':
            g_object_index = OBJECT_PARABOLOID;
            copy_color(g_object, g_object_paraboloid);
            break;

        case '7':
            g_object_index = OBJECT_CHAOS;
            copy_color(g_object, g_object_chaos);
            break;

        case 'A':
        case 'a':
            g_zSpeed -=SPEED_DELTA; 
            break;

        case 'D':
        case 'd':
            g_zSpeed += SPEED_DELTA;
            break;

        case 'W':
        case 'w':
            g_ySpeed -= SPEED_DELTA;
            break;

        case 'S':
        case 's':
            g_ySpeed += SPEED_DELTA;
            break;

        case 'T':
        case 't':
            if(g_morphing_types == point_morphing)
                g_morphing_types = line_morphing;
            else
                g_morphing_types = point_morphing;
            break;

        default:
            break;
    }
}

void calc_next_position(scene_object::point_type& src, const scene_object::point_type& end)
{
    GLfloat deltaX = g_move_delta;
    GLfloat deltaY = g_move_delta;
    GLfloat deltaZ = g_move_delta;

    if(src.m_x < end.m_x)
        src.m_x += deltaX;
    else
        src.m_x -= deltaX;

    if(src.m_y < end.m_y)
        src.m_y += deltaY;
    else
        src.m_y -= deltaY;

    if(src.m_z < end.m_z)
        src.m_z += deltaZ;
    else
        src.m_z -= deltaZ;
}

void cycle(int value)
{
    bool transform = false;

    for(size_t i = 0; i < g_object.points.size(); ++i) {
        if(!g_object.points[i].m_point.compare(g_objects[g_object_index].points[i].m_point, g_move_delta * 11 / 10)) {
            calc_next_position(g_object.points[i].m_point, g_objects[g_object_index].points[i].m_point);
            transform = true;
        }
    }
    g_morphing = transform;

    if(transform) {
        g_object_last = g_object;
    }

    g_yRotation += g_ySpeed;
    g_zRotation += g_zSpeed;

    glutPostRedisplay();
    glutTimerFunc(CYCLE_TIME, cycle, value);
}

int main(int argc, char** argv)
{
    info();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(640, 640);
    glutInitWindowPosition(300, 50);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(CYCLE_TIME, cycle, 0);
    glutMainLoop();

    return 0;
}