/**
 * ****************************************************************************
 * Copyright (c) 2015, Robert Lukierski.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * ****************************************************************************
 * Common includes/configuration.
 * ****************************************************************************
 */

#ifndef VISIONCORE_WRAPGL_COMMON_HPP
#define VISIONCORE_WRAPGL_COMMON_HPP

#include <functional>
#include <stdexcept>

#include <VisionCore/Platform.hpp>

#ifdef VISIONCORE_HAVE_GLBINDING
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glbinding/Meta.h>
#include <glbinding/Version.h>
#include <glbinding/ContextInfo.h>
#include <glbinding/ContextHandle.h>
using namespace gl;
#else // VISIONCORE_HAVE_GLBINDING
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>
typedef GLbitfield MemoryBarrierMask;
#endif // VISIONCORE_HAVE_GLBINDING

#ifndef WRAPGL_ERROR_CHECK_DISABLE
#define WRAPGL_CHECK_ERROR_SWITCH 1
#else // WRAPGL_ERROR_CHECK_DISABLE
#define WRAPGL_CHECK_ERROR_SWITCH 0
#endif // WRAPGL_ERROR_CHECK_DISABLE

/**
 * General TODO:
 * 
 * Add get/sets everywhere.
 *
 * Add copy/r-value constructors and operators.
 * 
 * Add more Core::Buffers compatibility.
 */

namespace vc
{

namespace wrapgl
{

namespace internal
{
    static constexpr std::array<GLuint,11> GLTypeToSize = 
    {{
        1, //  #define GL_BYTE 0x1400
        1, //  #define GL_UNSIGNED_BYTE 0x1401
        2, //  #define GL_SHORT 0x1402
        2, //  #define GL_UNSIGNED_SHORT 0x1403
        4, //  #define GL_INT 0x1404
        4, //  #define GL_UNSIGNED_INT 0x1405
        4, //  #define GL_FLOAT 0x1406
        2, //  #define GL_2_BYTES 0x1407
        3, //  #define GL_3_BYTES 0x1408
        4, //  #define GL_4_BYTES 0x1409
        8  //  #define GL_DOUBLE 0x140A  
    }};
    
    static constexpr GLuint getByteSize(GLenum dtype)
    {
        return GLTypeToSize[(int)dtype - (int)GL_BYTE];
    }
    
    template<typename ScalarT> struct GLTypeTraits;
    
    template<> struct GLTypeTraits<int8_t> { static constexpr GLenum opengl_data_type = GL_BYTE; };
    template<> struct GLTypeTraits<uint8_t> { static constexpr GLenum opengl_data_type = GL_UNSIGNED_BYTE; };
    template<> struct GLTypeTraits<int16_t> { static constexpr GLenum opengl_data_type = GL_SHORT; };
    template<> struct GLTypeTraits<uint16_t> { static constexpr GLenum opengl_data_type = GL_UNSIGNED_SHORT; };
    template<> struct GLTypeTraits<int32_t> { static constexpr GLenum opengl_data_type = GL_INT; };
    template<> struct GLTypeTraits<uint32_t> { static constexpr GLenum opengl_data_type = GL_UNSIGNED_INT; };
    template<> struct GLTypeTraits<float> { static constexpr GLenum opengl_data_type = GL_FLOAT; };
    template<> struct GLTypeTraits<double> { static constexpr GLenum opengl_data_type = GL_DOUBLE; };
    
    template<int chan> struct GLChannelTraits;
    
    template<> struct GLChannelTraits<1> { static constexpr GLenum opengl_data_format = GL_LUMINANCE; };
    template<> struct GLChannelTraits<2> { static constexpr GLenum opengl_data_format = GL_RG; };
    template<> struct GLChannelTraits<3> { static constexpr GLenum opengl_data_format = GL_RGB; };
    template<> struct GLChannelTraits<4> { static constexpr GLenum opengl_data_format = GL_RGBA; };
    
    template<typename T>
    struct OpenGLGetter;
    
    template<> struct OpenGLGetter<GLboolean>
    {
        static inline GLboolean run(GLenum param)
        {
            GLboolean val;
            glGetBooleanv(param, &val);
            return val;
        }
        
        static inline void runArray(GLenum param, GLuint idx, GLboolean* data)
        {
            glGetBooleani_v(param, idx, data);
        }
    };
    
    template<> struct OpenGLGetter<GLfloat>
    {
        static inline GLfloat run(GLenum param)
        {
            GLfloat val;
            glGetFloatv(param, &val);
            return val;
        }
        
        static inline void runArray(GLenum param, GLuint idx, GLfloat* data)
        {
            glGetFloati_v(param, idx, data);
        }
    };
    
    template<> struct OpenGLGetter<GLdouble>
    {
        static inline GLdouble run(GLenum param)
        {
            GLdouble val;
            glGetDoublev(param, &val);
            return val;
        }
        
        static inline void runArray(GLenum param, GLuint idx, GLdouble* data)
        {
            glGetDoublei_v(param, idx, data);
        }
    };
    
    template<> struct OpenGLGetter<GLint>
    {
        static inline GLint run(GLenum param)
        {
            GLint val;
            glGetIntegerv(param, &val);
            return val;
        }
        
        static inline void runArray(GLenum param, GLuint idx, GLint* data)
        {
            glGetIntegeri_v(param, idx, data);
        }
    };
    
    template<> struct OpenGLGetter<GLint64>
    {
        static inline GLint64 run(GLenum param)
        {
            GLint64 val;
            glGetInteger64v(param, &val);
            return val;
        }
        
        static inline void runArray(GLenum param, GLuint idx, GLint64* data)
        {
            glGetInteger64i_v(param, idx, data);
        }
    };
    
    template<typename T>
    static inline T getParameter(GLenum param) { return OpenGLGetter<T>::run(param); }
    
    template<typename T>
    static inline void getParameterArray(GLenum param, GLuint idx, T* data) { return OpenGLGetter<T>::runArray(param,idx,data); }
    
    const char* getErrorString(const GLenum err);
}
// 
template<typename WrapGLT>
struct ScopeBinder
{
    ScopeBinder() = delete;
    ScopeBinder(const ScopeBinder&) = delete;
    ScopeBinder(ScopeBinder&&) = delete;
    ScopeBinder& operator=(const ScopeBinder&) = delete;
    ScopeBinder& operator=(ScopeBinder&&) = delete;
    
    inline ScopeBinder(const WrapGLT& aobj) : obj(aobj)
    {
        obj.bind();
    }
    
    inline ~ScopeBinder()
    {
        obj.unbind();
    }
    
    const WrapGLT& obj;
};

struct Debug
{
    typedef std::function<void(GLenum source, GLenum type, GLuint id, GLenum severity, const std::string& message)> CallbackT;
    static void enable();
    static void disable();
    static void registerCallback(const CallbackT& fun);
    static void insert(GLenum source, GLenum type, GLuint id, GLenum severity, const std::string& msg);   
};

class OpenGLException : public std::exception
{
public:
    inline OpenGLException(GLenum err = GL_NO_ERROR, const char* fn = nullptr, int line = 0)
        : errcode(err), src_fn(fn), src_line(line)
    { 
        
    }
    
    virtual ~OpenGLException() throw() { }
    virtual const char* what() const throw()  
    { 
        std::stringstream ss;
        ss << "OpenGLException: " << (int)errcode << " / " << internal::getErrorString(errcode) 
           << " at " << src_fn << ":" << src_line;
        return ss.str().c_str(); 
    }
    GLenum getError() const { return errcode; }
private:
    GLenum errcode;
    const char* src_fn;
    int src_line;
};

namespace internal
{
    static inline void checkError(const char* fn, const int line)
    {
        const GLenum rc = glGetError();
        if(rc != GL_NO_ERROR) 
        {
            throw OpenGLException(rc, fn, line);
        }
    }
}

}

}

#define WRAPGL_CHECK_ERROR() do { if(WRAPGL_CHECK_ERROR_SWITCH) { vc::wrapgl::internal::checkError(__FILE__,__LINE__); } } while (0)

#endif // VISIONCORE_WRAPGL_COMMON_HPP
