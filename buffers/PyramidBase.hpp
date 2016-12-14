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
 * Base of a 2D Host/Device Buffer Pyramid.
 * ****************************************************************************
 */
#ifndef CORE_PYRAMID_BASE_HPP
#define CORE_PYRAMID_BASE_HPP

#include <Platform.hpp>

namespace core
{

// Power of two pyramid.
template<template<class, template<typename> class> class ViewT, typename T, std::size_t Levels, template<typename = T> class Target>
class PyramidViewBase
{
public:    
    typedef ViewT<T,Target> ViewType;
    static const std::size_t LevelCount = Levels;
    typedef T ValueType;
    
    EIGEN_DEVICE_FUNC inline PyramidViewBase() { }
    
    EIGEN_DEVICE_FUNC inline ~PyramidViewBase() { }
    
    EIGEN_DEVICE_FUNC inline PyramidViewBase(const PyramidViewBase& pyramid)
    {
        for(std::size_t l = 0 ; l < LevelCount ; ++l) 
        {
            imgs[l] = pyramid.imgs[l];
        }
    }
    
    EIGEN_DEVICE_FUNC inline PyramidViewBase(PyramidViewBase&& pyramid) 
    {
        for(std::size_t l = 0 ; l < LevelCount ; ++l) 
        {
            imgs[l] = std::move(pyramid.imgs[l]);
        }
    }
    
    EIGEN_DEVICE_FUNC inline PyramidViewBase& operator=(const PyramidViewBase& pyramid)
    {
        for(std::size_t l = 0 ; l < LevelCount ; ++l) 
        {
            imgs[l] = pyramid.imgs[l];
        }

        return *this;
    }
    
    EIGEN_DEVICE_FUNC inline PyramidViewBase& operator=(PyramidViewBase&& pyramid)
    {
        for(std::size_t l = 0 ; l < LevelCount ; ++l) 
        {
            imgs[l] = std::move(pyramid.imgs[l]);
        }
        
        return *this;
    }

    EIGEN_DEVICE_FUNC inline void swap(PyramidViewBase& pyramid)
    {
        for(std::size_t l = 0 ; l < LevelCount ; ++l) 
        {
            imgs[l].swap(pyramid.imgs[l]);
        }
    }

    EIGEN_DEVICE_FUNC inline ViewType& operator[](std::size_t i)
    {
        return imgs[i];
    }

    EIGEN_DEVICE_FUNC inline const ViewType& operator[](std::size_t i) const
    {
        return imgs[i];
    }
    
    EIGEN_DEVICE_FUNC inline ViewType& operator()(std::size_t i)
    {
        return imgs[i];
    }
    
    EIGEN_DEVICE_FUNC inline const ViewType& operator()(std::size_t i) const
    {
        return imgs[i];
    }

    template<std::size_t SubLevels>
    EIGEN_DEVICE_FUNC inline PyramidViewBase<ViewT, T, SubLevels,Target> subPyramid(std::size_t startLevel)
    {
        PyramidViewBase<ViewT, T, SubLevels,Target> pyr;

        for(std::size_t l = 0 ; l < SubLevels; ++l) 
        {
            pyr.imgs[l] = imgs[startLevel+l];
        }

        return pyr;
    }
protected:
    ViewType imgs[LevelCount];
};

}

#endif // CORE_PYRAMID_BASE_HPP
