/**
 * ****************************************************************************
 * Copyright (c) 2017, Robert Lukierski.
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
 * VAO.
 * ****************************************************************************
 */

#ifndef VISIONCORE_WRAPGL_VERTEX_ARRAY_OBJECT_IMPL_HPP
#define VISIONCORE_WRAPGL_VERTEX_ARRAY_OBJECT_IMPL_HPP

vc::wrapgl::VertexArrayObject::VertexArrayObject() : vaoid(0)
{
    create();
}

void vc::wrapgl::VertexArrayObject::create()
{
    destroy();
    
    glGenVertexArrays(1, &vaoid);
    WRAPGL_CHECK_ERROR();
}

void vc::wrapgl::VertexArrayObject::destroy()
{
    if(vaoid != 0)
    {
        glDeleteVertexArrays(1, &vaoid);
        WRAPGL_CHECK_ERROR();
        vaoid = 0;
    }
}

bool vc::wrapgl::VertexArrayObject::isValid() const 
{ 
    return vaoid != 0; 
}

void vc::wrapgl::VertexArrayObject::bind() const
{
    glBindVertexArray(vaoid);
    WRAPGL_CHECK_ERROR();
}

void vc::wrapgl::VertexArrayObject::unbind() const
{
    glBindVertexArray(0);
    WRAPGL_CHECK_ERROR();
}

GLuint vc::wrapgl::VertexArrayObject::id() const 
{ 
    return vaoid; 
}

void vc::wrapgl::VertexArrayObject::setDivisor(GLuint bindingindex, GLuint divisor)
{
    glVertexArrayBindingDivisor(vaoid, bindingindex, divisor);
    WRAPGL_CHECK_ERROR();
}

#endif // VISIONCORE_WRAPGL_VERTEX_ARRAY_OBJECT_IMPL_HPP
