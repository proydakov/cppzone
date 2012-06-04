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

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <AL/alut.h>

#include "sound.h"

namespace sound {

system* system::m_instance = 0;

struct sound_info 
{
    std::string   m_filename;
    unsigned int  m_id;
    unsigned int  m_rate;
    unsigned int  m_format;
};

typedef std::map<system::sound_id, sound_info> sound_buffers;

struct system::d
{
    d() :
        m_pDevice(0),
        m_pContext(0),
        m_buffers(new sound_buffers)
    {
    }
    
    ~d()
    {
        assert(!m_pDevice);
        assert(!m_pContext);
        delete m_buffers;
    }
    
    ALCdevice*      m_pDevice;
    ALCcontext*     m_pContext;
    sound_buffers*  m_buffers;
};

system* system::get_instance()
{
    if(!m_instance) {
        m_instance = new system;
        atexit(remove);
    }
    return m_instance;   
}

void system::remove()
{
    delete m_instance;
}

system::system()
{
    m_d = new d;
    
    bool res = init();
    
    if(!res) {
        delete m_d;
        throw std::runtime_error("Init sound system error.");
    }
}

system::~system()
{
    destroy();
    
    delete m_d;
}

bool system::init()
{
    if(m_d->m_pDevice) {
        return true;
    }
    
    m_d->m_pDevice = alcOpenDevice(NULL);
    
    if(!m_d->m_pDevice) {
        std::cerr << "Default sound device not present" << std::endl;
        return false;
    }
    
    m_d->m_pContext = alcCreateContext(m_d->m_pDevice, NULL);
    if(!check_alc_error()) {
        return false;
    }
    
    alcMakeContextCurrent(m_d->m_pContext);
    
    ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerOri[] = { 1.0, 1.0, 0.0,  0.0, 0.0, 1.0 };
    
    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);
    
    return true;
}

void system::destroy()
{
    if(!m_d->m_pDevice)
        return;
    
    for(sound_buffers::iterator i = m_d->m_buffers->begin(); i != m_d->m_buffers->end(); ++i) {
        alDeleteBuffers(1, &i->second.m_id);
    }
    alcMakeContextCurrent(NULL);
    alcDestroyContext(m_d->m_pContext);
    alcCloseDevice(m_d->m_pDevice);
    m_d->m_pDevice = 0;
    m_d->m_pContext = 0;
}

bool system::check_alc_error()
{
    ALenum ErrCode;
    std::string err = "ALC error: ";
    if((ErrCode = alcGetError(m_d->m_pDevice)) != ALC_NO_ERROR) {
        err += (char *)alcGetString(m_d->m_pDevice, ErrCode);
        std::cerr << err << std::endl;
        return AL_FALSE;
    }
    return AL_TRUE;
}

bool system::check_al_error()
{
    ALenum ErrCode;
    std::string err("OpenAL error: ");
    if((ErrCode = alGetError()) != AL_NO_ERROR) {
        char* text = (char*)alGetString(ErrCode);
        std::cerr << err << text << std::endl;
        return AL_FALSE;
    }
    return AL_TRUE;
}

system::sound_id system::load_wav(const std::string& filename)
{
    sound_id bad_id = -1;
    
    sound_info  buffer;
    
    ALenum    format;
    ALvoid*   data;
    ALsizei   size;
    ALsizei   freq;
    ALboolean loop;
    ALuint    buf_id = 0;
    
    buffer.m_filename = filename;
    sound_buffers::const_iterator end_it = m_d->m_buffers->end();
    for(sound_buffers::const_iterator it = m_d->m_buffers->begin(); it != end_it; ++it) {
        if(it->second.m_filename == filename) {
            buf_id = it->first;
            break;
        }
    }
    
    if(!buf_id) {
        alGenBuffers(1, &buffer.m_id);
        if(!check_al_error()) {
            return false;
        }
        alutLoadWAVFile((ALbyte *)filename.c_str(), &format, &data, &size, &freq, &loop);
        if (!check_al_error()) {
            return bad_id;
        }
        
        buffer.m_format = format;
        buffer.m_rate   = freq;
        
        alBufferData(buffer.m_id, format, data, size, freq);
        if (!check_al_error()) {
            return bad_id;
        }
        
        alutUnloadWAV(format, data, size, freq);
        if (!check_al_error()) {
            return bad_id;
        }
        m_d->m_buffers->insert(sound_buffers::value_type(buf_id, buffer));
    }
    else {
        buffer = (*m_d->m_buffers)[buf_id];
    }
    
    return buffer.m_id;
}

//-----------------------------------------------------------------------------

void listener::move(double x, double y, double z)
{
    (void)x;
    (void)y;
    (void)z;
}

//-----------------------------------------------------------------------------

sound::sound() :
    m_source_id(0),
    m_x(0),
    m_y(0),
    m_z(0)
{
    system::get_instance();
}

sound::~sound()
{
    close();
}

bool sound::open(const std::string& filename, bool looped)
{
    std::ifstream a(filename.c_str());
    if(!a.is_open()) { 
        return false;
    }
    a.close();
    
    ALuint source_id = m_source_id;
    alGenSources(1, &source_id);
    m_source_id = source_id;
    if(!system::get_instance()->check_al_error()) {
        return false;
    }
    
    ALfloat pos[] = { (ALfloat) m_x, (ALfloat) m_y, (ALfloat) m_z };
    ALfloat vel[] = { 0.0f, 0.0f, 0.0f };
    
    alSourcef (m_source_id, AL_PITCH,    1.0f);
    alSourcef (m_source_id, AL_GAIN,     1.0f);
    alSourcefv(m_source_id, AL_POSITION, pos);
    alSourcefv(m_source_id, AL_VELOCITY, vel);
    alSourcei (m_source_id, AL_LOOPING,  looped);
    
    system::sound_id id = system::get_instance()->load_wav(filename);
    if(id == -1) {
        return false;
    }
    
    alSourcei(m_source_id, AL_BUFFER, id);
    
    return true;
}

void sound::close()
{
    alSourceStop(m_source_id);
    if(alIsSource(m_source_id)) {
        ALuint source_id = m_source_id;
        alDeleteSources(1, &source_id);
        m_source_id = source_id;
    }
}

void sound::play()
{
    alSourcePlay(m_source_id);
}

void sound::stop()
{
    alSourceStop(m_source_id);
}

void sound::move(double x, double y, double z)
{
    m_x = x;
    m_y = y;
    m_z = z;
    
    ALfloat pos[3] = { (ALfloat) x, (ALfloat) y, (ALfloat) z };
    alSourcefv(m_source_id, AL_POSITION, pos);
}

} // sound
