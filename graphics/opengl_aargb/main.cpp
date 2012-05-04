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

#include <string>
#include <iostream>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

GLdouble g_rotAngle = 0;

const std::string comment = "";

void init()
{
    GLfloat values[2];
    
    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, values);
    std::cout << "GL_LINE_WIDTH_GRANULARITY : " << values[0] << std::endl;
    
    glGetFloatv(GL_LINE_WIDTH_RANGE, values);
    std::cout << "GL_LINE_WIDTH_RANGE : " << values[0] << std::endl;
    
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5);
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void display()
{
    
}

int main()
{
    return 0;
}
