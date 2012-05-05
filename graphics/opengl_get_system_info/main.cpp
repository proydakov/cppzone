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
#include <fstream>
#include <iostream>

#include <GL/glut.h>

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    const GLubyte* glVendor = glGetString(GL_VENDOR);
    const GLubyte* glRenderer = glGetString(GL_RENDERER);
    const GLubyte* glVersion = glGetString(GL_VERSION);
    GLubyte* glExtensions = const_cast<GLubyte*>(glGetString(GL_EXTENSIONS));

    if(!glVendor || !glRenderer || !glVersion || !glExtensions) {
        std::cout << "Not found the required version of OpenGL." << std::endl;
        return 1;
    }

    int counter = 0;
    for(int i = 0; glExtensions[i]; ++i) {
        if(glExtensions[i] == ' ') {
            glExtensions[i] = '\n';
            ++counter;
        }
    }

    std::ofstream file;

    std::string fileName("opengl_system_info.txt");
    file.open(fileName.c_str());
    if(file.is_open()) {
        file << "GL_VENDOR     : " << glVendor << "\n\n"
            << "GL_RENDERER   : " << glRenderer << "\n\n"
            << "GL_VERSION    : " << glVersion << "\n\n"
            << "GL_EXTENSIONS : " << counter << "\n\n"
            << glExtensions << std::endl;
    }
    else {
        std::cout << "Can not create : " << fileName << " error !!!" << std::endl;
        return 1;
    }
    return 0;
}