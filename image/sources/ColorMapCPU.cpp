/**
 * ****************************************************************************
 * Copyright (c) 2016, Robert Lukierski.
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
 * Color Maps.
 * ****************************************************************************
 */

#include <image/ColorMap.hpp>
#include <image/sources/ColorMapDefs.hpp>
#include <LaunchUtils.hpp>

template<typename T>
float3 core::image::getColorMapValue(ColorMap cm, const T& vmin, const T& vmax, const T& val)
{
    return getColorMapValuePreload(getColorMapSize(cm), getColorMapData(cm), vmin, vmax, val);
}

std::size_t getColorMapSizeForwarder(core::image::ColorMap cm) { return getColorMapSize(cm); }

template<typename T, typename TOUT, typename Target>
void core::image::createColorMap(ColorMap cm, const core::Buffer2DView<T,Target>& img_in, const T& vmin, const T& vmax, core::Buffer2DView<TOUT,Target>& img_out)
{
    const std::size_t cms = getColorMapSize(cm);
    const float3* data = getColorMapData(cm);
    
    core::launchParallelFor(img_in.width(), img_in.height(), [&](std::size_t x, std::size_t y)
    {
        if(img_in.inBounds(x,y) && img_out.inBounds(x,y))
        {
            const T& val_in = img_in(x,y);
            TOUT& val_out = img_out(x,y);
            
            float3 result = getColorMapValuePreload(cms, data, vmin, vmax, val_in);
            val_out = ConvertToTarget<TOUT>::run(result);
        }
    });
}

#define GENERATE_IMPL(TIN,TOUT)\
template void core::image::createColorMap<TIN,TOUT,core::TargetHost>(ColorMap cm, const core::Buffer2DView<TIN,core::TargetHost>& img_in, const TIN& vmin, const TIN& vmax, core::Buffer2DView<TOUT,core::TargetHost>& img_out);

GENERATE_IMPL(float,float3)
GENERATE_IMPL(float,float4)
