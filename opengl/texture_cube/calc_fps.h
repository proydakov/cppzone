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

#ifndef I_CALC_FPS_H
#define I_CALC_FPS_H

#include <time.h>

class calc_fps
{
public:
    calc_fps(unsigned cycle_time = 2) :
      m_base_time(0),
      m_cycle_time(cycle_time),
      m_frames(0),
      m_fps(0)
      {
          m_base_time = time(0);
      }

      void calc()
      {
          ++m_frames;
          time_t t = time(0);
          if(t - m_base_time <= m_cycle_time) {
              return;
          }
          double seconds = (t - m_base_time) / 1.0;
          double fps = m_frames / seconds;

          m_base_time = t;
          m_frames = 0;
          m_fps = static_cast<unsigned>(fps);
      }

      unsigned get_fps()
      {
          return m_fps;
      }

private:
    time_t m_base_time;
    unsigned m_cycle_time;
    unsigned m_frames;
    unsigned m_fps;
};

#endif // I_CALC_FPS_H
