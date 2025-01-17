#ifndef _ES31FFBOTESTUTIL_HPP
#define _ES31FFBOTESTUTIL_HPP
/*-------------------------------------------------------------------------
 * drawElements Quality Program OpenGL ES 3.1 Module
 * -------------------------------------------------
 *
 * Copyright 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *//*!
 * \file
 * \brief FBO test utilities.
 *//*--------------------------------------------------------------------*/

#include "tcuDefs.hpp"
#include "sglrContext.hpp"
#include "gluShaderUtil.hpp"
#include "tcuTexture.hpp"
#include "tcuMatrix.hpp"
#include "tcuRenderTarget.hpp"

#include <vector>

namespace deqp
{
namespace gles31
{
namespace Functional
{
namespace FboTestUtil
{

// \todo [2012-04-29 pyry] Clean up and name as SglrUtil

// Helper class for constructing DataType vectors.
struct DataTypes
{
    std::vector<glu::DataType> vec;
    DataTypes &operator<<(glu::DataType type)
    {
        vec.push_back(type);
        return *this;
    }
};

// Shaders.

class Texture2DShader : public sglr::ShaderProgram
{
public:
    Texture2DShader(const DataTypes &samplerTypes, glu::DataType outputType,
                    const tcu::Vec4 &outScale = tcu::Vec4(1.0f), const tcu::Vec4 &outBias = tcu::Vec4(0.0f));
    ~Texture2DShader(void)
    {
    }

    void setUnit(int samplerNdx, int unitNdx);
    void setTexScaleBias(int samplerNdx, const tcu::Vec4 &scale, const tcu::Vec4 &bias);
    void setOutScaleBias(const tcu::Vec4 &scale, const tcu::Vec4 &bias);

    void setUniforms(sglr::Context &context, uint32_t program) const;

    void shadeVertices(const rr::VertexAttrib *inputs, rr::VertexPacket *const *packets, const int numPackets) const;
    void shadeFragments(rr::FragmentPacket *packets, const int numPackets,
                        const rr::FragmentShadingContext &context) const;

private:
    struct Input
    {
        int unitNdx;
        tcu::Vec4 scale;
        tcu::Vec4 bias;
    };

    std::vector<Input> m_inputs;
    tcu::Vec4 m_outScale;
    tcu::Vec4 m_outBias;

    const glu::DataType m_outputType;
};

class TextureCubeArrayShader : public sglr::ShaderProgram
{
public:
    TextureCubeArrayShader(glu::DataType samplerType, glu::DataType outputType, glu::GLSLVersion glslVersion);
    ~TextureCubeArrayShader(void)
    {
    }

    void setLayer(int layer);
    void setFace(tcu::CubeFace face);
    void setTexScaleBias(const tcu::Vec4 &scale, const tcu::Vec4 &bias);

    void setUniforms(sglr::Context &context, uint32_t program) const;

    void shadeVertices(const rr::VertexAttrib *inputs, rr::VertexPacket *const *packets, const int numPackets) const;
    void shadeFragments(rr::FragmentPacket *packets, const int numPackets,
                        const rr::FragmentShadingContext &context) const;

private:
    tcu::Vec4 m_texScale;
    tcu::Vec4 m_texBias;
    int m_layer;
    tcu::Mat3 m_coordMat;

    const glu::DataType m_outputType;
};

// Framebuffer incomplete exception.
class FboIncompleteException : public tcu::TestError
{
public:
    FboIncompleteException(uint32_t reason, const char *file, int line);
    virtual ~FboIncompleteException(void) throw()
    {
    }

    uint32_t getReason(void) const
    {
        return m_reason;
    }

private:
    uint32_t m_reason;
};

// Utility functions

glu::DataType getFragmentOutputType(const tcu::TextureFormat &format);
tcu::TextureFormat getFramebufferReadFormat(const tcu::TextureFormat &format);

const char *getFormatName(uint32_t format);

void clearColorBuffer(sglr::Context &ctx, const tcu::TextureFormat &format, const tcu::Vec4 &value);
void readPixels(sglr::Context &ctx, tcu::Surface &dst, int x, int y, int width, int height,
                const tcu::TextureFormat &format, const tcu::Vec4 &scale, const tcu::Vec4 &bias);

tcu::RGBA getFormatThreshold(const tcu::TextureFormat &format);
tcu::RGBA getFormatThreshold(const uint32_t glFormat);

} // namespace FboTestUtil
} // namespace Functional
} // namespace gles31
} // namespace deqp

#endif // _ES31FFBOTESTUTIL_HPP
