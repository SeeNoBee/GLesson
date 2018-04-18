#pragma once

#include <glint/InternalTextureFormat.h>

namespace opengl
{
using namespace gl;
using namespace std;

static const GLenum baseExtFormats[4] = {
	GL_RED,
	GL_RG,
	GL_RGB,
	GL_RGBA
};

static const GLenum intExtFormats[4] = {
	GL_RED_INTEGER,
	GL_RG_INTEGER,
	GL_RGB_INTEGER,
	GL_RGBA_INTEGER
};

class ExternalFormat
{
public:
	template <typename extType, unsigned char channels, TextureType tType>
	static GLenum format();

	template <typename extType, TextureType tType>
	static GLenum format(unsigned char channels);

	template <typename extType>
	static GLenum type();

	template <>
	static GLenum type<GLfloat>();

	template <>
	static GLenum type<GLint>();

	template <>
	static GLenum type<GLuint>();

	template <>
	static GLenum type<GLshort>();

	template <>
	static GLenum type<GLushort>();

	template <>
	static GLenum type<GLbyte>();

	template <>
	static GLenum type<GLubyte>();

	virtual GLenum storedFormat(unsigned char channels) const = 0;
	virtual GLenum storedType() const = 0;

	virtual ~ExternalFormat() {}
};

template <typename extType, unsigned char channels, TextureType tType>
class ExtChFormatStore : public ExternalFormat
{
public:
	static_assert((channels > 0) && (channels <= 4), "channels must be in range [1, 4]");
	GLenum storedFormat(unsigned char channels) const override;
	GLenum storedType() const override;
	~ExtChFormatStore() {}
};

template <typename extType, TextureType tType>
class ExtFormatStore : public ExternalFormat
{
public:
	GLenum storedFormat(unsigned char channels) const override;
	GLenum storedType() const override;
	~ExtFormatStore() {}
};

/*******************************************Implementation*******************************************/

//ExternalFormat
template <typename extType, unsigned char channels, TextureType tType>
static GLenum ExternalFormat::format()
{
	static_assert((channels > 0) && (channels <= 4), "channels must be in range [1, 4]");
	static_assert((tType == TextureType::TT_BASE) || is_integral<extType>::value, "invalid external format for this texture type");
	return (tType == TextureType::TT_BASE) ? baseExtFormats[channels - 1] : intExtFormats[channels - 1];
}

template <typename extType, TextureType tType>
static GLenum ExternalFormat::format(unsigned char channels)
{
	static_assert((tType == TextureType::TT_BASE) || is_integral<extType>::value, "invalid external format for this texture type");
	return (tType == TextureType::TT_BASE) ? baseExtFormats[channels - 1] : intExtFormats[channels - 1];
}

template <>
static GLenum ExternalFormat::type<GLfloat>()
{
	return GL_FLOAT;
}

template <>
static GLenum ExternalFormat::type<GLint>()
{
	return GL_INT;
}

template <>
static GLenum ExternalFormat::type<GLuint>()
{
	return GL_UNSIGNED_INT;
}

template <>
static GLenum ExternalFormat::type<GLshort>()
{
	return GL_SHORT;
}

template <>
static GLenum ExternalFormat::type<GLushort>()
{
	return GL_UNSIGNED_SHORT;
}

template <>
static GLenum ExternalFormat::type<GLbyte>()
{
	return GL_BYTE;
}

template <>
static GLenum ExternalFormat::type<GLubyte>()
{
	return GL_UNSIGNED_BYTE;
}

//ExtChFormatStorage
template <typename extType, unsigned char channels, TextureType tType>
GLenum ExtChFormatStore<extType, channels, tType>::storedFormat(unsigned char _channels) const
{
	return ExternalFormat::format<extType, channels, tType>();
}

template <typename extType, unsigned char channels, TextureType tType>
GLenum ExtChFormatStore<extType, channels, tType>::storedType() const
{
	return ExternalFormat::type<extType>();
}

//ExtFormatStorage
template <typename extType, TextureType tType>
GLenum ExtFormatStore<extType, tType>::storedFormat(unsigned char channels) const
{
	return ExternalFormat::format<extType, tType>(channels);
}

template <typename extType, TextureType tType>
GLenum ExtFormatStore<extType, tType>::storedType() const
{
	return ExternalFormat::type<extType>();
}

}
