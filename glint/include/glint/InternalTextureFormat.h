#pragma once

#include <glbinding\gl\gl.h>
#ifndef TEX_DEFAULT_CHANNELS_COUNT
#define TEX_DEFAULT_CHANNELS_COUNT 3
#endif

namespace opengl
{
using namespace gl;

enum class TextureType : unsigned char
{
	TT_BASE,
	TT_INTEGRAL,
};

static const TextureType TT_BASE = TextureType::TT_BASE;
static const TextureType TT_INTEGRAL = TextureType::TT_INTEGRAL;

template <TextureType type>
class InternalFormat
{
public:
	inline unsigned char getChannels() const;

	inline unsigned char getDepth() const;

	inline GLenum getFormat() const;

	//TODO: glGetTexLevelParameter
protected:
	InternalFormat(
		unsigned char _channels,
		unsigned char _depth,
		GLenum _format = GL_R8
	) :
		channels(_channels),
		depth(_depth),
		format(_format) {}

	GLenum format; //TODO: можно не хранить, а брать из текстуры
	unsigned char channels;
	unsigned char depth;
};

static const GLenum baseIntFormats[8] = {
	GL_R8,
	GL_R16,
	GL_RG8,
	GL_RG16,
	GL_RGB8,
	GL_RGB16,
	GL_RGBA8,
	GL_RGBA16,
};

template <unsigned char _channels = TEX_DEFAULT_CHANNELS_COUNT, unsigned char _depth = 2>
class BaseFormat : public InternalFormat<TextureType::TT_BASE>
{
public:
	BaseFormat():
		InternalFormat<TextureType::TT_BASE>(
			_channels,
			_depth,
			baseIntFormats[2 * (_channels - 1) + (_depth - 1)])
	{
		static_assert((_channels > 0) && (_channels <= 4), "for base texture format channels must be in range [1, 4]");
		static_assert((_depth == 1) || (_depth == 2), "for base texture format depth may be only 1 or 2 bytes"); 
	}
};

static GLenum baseSignedIntFormats[8] = {
	GL_R8_SNORM,
	GL_R16_SNORM,
	GL_RG8_SNORM,
	GL_RG16_SNORM,
	GL_RGB8_SNORM,
	GL_RGB16_SNORM,
	GL_RGBA8_SNORM,
	GL_RGBA16_SNORM
};

template <unsigned char _channels = TEX_DEFAULT_CHANNELS_COUNT, unsigned char _depth = 2>
class BaseSignedFormat : public InternalFormat<TextureType::TT_BASE>
{
public:
	BaseSignedFormat() :
		InternalFormat<TextureType::TT_BASE>(
			_channels,
			_depth,
			baseSignedIntFormats[2 * (_channels - 1) + (_depth - 1)])
	{
		static_assert((_channels > 0) && (_channels <= 4), "for signed base texture format channels must be in range [1, 4]");
		static_assert((_depth == 1) || (_depth == 2), "for signed base texture format depth may be only 1 or 2 bytes");
	}
};

static const GLenum floatIntFormats[8] = {
	GL_R16F,
	GL_R32F,
	GL_RG16F,
	GL_RG32F,
	GL_RGB16F,
	GL_RGB32F,
	GL_RGBA16F,
	GL_RGBA32F
};

template <unsigned char _channels = TEX_DEFAULT_CHANNELS_COUNT, unsigned char _depth = 4>
class FloatFormat : public InternalFormat<TextureType::TT_BASE>
{
public:
	FloatFormat():
		InternalFormat<TextureType::TT_BASE>(
			_channels,
			_depth,
			floatIntFormats[(2 * (_channels - 1)) + ((_depth / 2) - 1)]
			)
	{
		static_assert((_channels > 0) && (_channels <= 4), "for float texture format channels must be in range [1, 4]");
		static_assert((_depth == 2) || (_depth == 4), "for float texture format depth may be only 2 or 4 bytes");
	}
};

static const GLenum integralIntFormats[12] = {
	GL_R8I,
	GL_R16I,
	GL_R32I,
	GL_RG8I,
	GL_RG16I,
	GL_RG32I,
	GL_RGB8I,
	GL_RGB16I,
	GL_RGB32I,
	GL_RGBA8I,
	GL_RGBA16I,
	GL_RGBA32I
};

template <unsigned char _channels = TEX_DEFAULT_CHANNELS_COUNT, unsigned char _depth = 4>
class IntegralFormat: public InternalFormat<TextureType::TT_INTEGRAL>
{
public:
	IntegralFormat():
		InternalFormat<TextureType::TT_INTEGRAL>(
			_channels,
			_depth,
			integralIntFormats[(3 * (_channels - 1)) + (_depth / 2)]
			)
	{
		static_assert((_channels > 0) && (_channels <= 4), "for integer texture format channels must be in range [1, 4]");
		static_assert((_depth == 2) || (_depth == 4) || (_depth == 1), "for integer texture format depth may be only 1 or 2 or 4 bytes");
	}
};

static const GLenum unsignedIntFormats[12] = {
	GL_R8UI,
	GL_R16UI,
	GL_R32UI,
	GL_RG8UI,
	GL_RG16UI,
	GL_RG32UI,
	GL_RGB8UI,
	GL_RGB16UI,
	GL_RGB32UI,
	GL_RGBA8UI,
	GL_RGBA16UI,
	GL_RGBA32UI
};

template <unsigned char _channels = TEX_DEFAULT_CHANNELS_COUNT, unsigned char _depth = 4>
class UnsignedFormat: public InternalFormat<TextureType::TT_INTEGRAL>
{
public:
	UnsignedFormat():
		InternalFormat<TextureType::TT_INTEGRAL>(
			_channels,
			_depth,
			unsignedIntFormats[(3 * (_channels - 1)) + (_depth / 2)]
			)
	{
		static_assert((_channels > 0) && (_channels <= 4), "for unsigned texture format channels must be in range [1, 4]");
		static_assert((_depth == 2) || (_depth == 4) || (_depth == 1), "for unsigned texture format depth may be only 1 or 2 or 4 bytes");
	}
};

/*******************************************Implementation*******************************************/

template <TextureType type>
unsigned char InternalFormat<type>::getChannels() const
{
	return channels;
}

template <TextureType type>
unsigned char InternalFormat<type>::getDepth() const
{
	return depth;
}

template <TextureType type>
GLenum InternalFormat<type>::getFormat() const
{
	return format;
}

}
