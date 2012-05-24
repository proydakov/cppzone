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

#include <map>
#include <string>

namespace sound {

class system
{
public:
    typedef int sound_id;
    
public:
    static system* get_instance();
    
    bool check_alc_error();
    bool check_al_error();
    
    sound_id load_wav(const std::string& filename);
    
private:
    system();
    ~system();
    
    bool init();
    void destroy();
    
    static void remove();
    
private:    
    static system* m_instance;
    
    struct d;
    d* m_d;
};


class listener
{
public:
    static void move(double x, double y, double z);
    
private:
    static double m_x;
    static double m_y;
    static double m_z;
};


class sound
{
public:
    sound();
    ~sound();
    
    bool open(const std::string& filename, bool looped);
    void close();
    
    void play();
    void stop();
    
    void move(double x, double y, double z);
    
private:  
    int m_source_id;
    
    double m_x;
    double m_y;
    double m_z;
};

} // sound
