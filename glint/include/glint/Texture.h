#pragma once

#include <glint/SamplerData.h>
#include <glint/ExternalTextureFormat.h>
#include <limits>
#include <iostream>

namespace opengl
{
using namespace gl;
using namespace std;

//TODO: ��������� ��� ���������, �������� �� ������ �������� ��� ���
//TODO: Pixel transfer parameters

enum class ImageAccess
{
	IA_WRITE = int(GL_WRITE_ONLY),
	IA_READ = int(GL_READ_ONLY),
	IA_READ_WRITE = int(GL_READ_WRITE)
};

//TODO: ���������� �� temlate <TextureType type, size>
//TODO: ������� ��������� ��������� ��� �������� map c ��������� ������ �����������

template <TextureType type>
class Texture
{
public:
	Texture(const InternalFormat<type> &format);
	Texture();

	template <typename extType, unsigned char channels>
	extType* map();

	//TODO: const extType* map() const;

	template <typename extType>
	extType* map();

	//TODO: const extType* map() const;

	void flush();

	void update();

	const GLuint getTexture() const;

	template <unsigned char channels, typename extType>
	void update(const extType *data);

	template <typename extType>
	void update(const extType *data);

	template <typename extType>
	void setNewFormat(const InternalFormat<type> &format, extType r = extType(0), extType g = extType(0), extType b = extType(0), extType a = extType(0));

	template <SamplerDataType ... sdts>
	void setSamplerParametres(const SamplerData<sdts>&...samplers);

	template <SamplerDataType sdt>
	SamplerData<sdt> getSamplerParametres() const;

	bool isDefault() const;
	virtual void reset();
	unsigned int size() const;

	const InternalFormat<type>& getFormat() const;

	void bindTexture(unsigned int unit) const;
	void bindImage(unsigned int unit, ImageAccess acces = ImageAccess::IA_READ_WRITE) const;

	template <typename extType>
	void fill(
		extType r,
		extType g = numeric_limits<extType>::lowest(),
		extType b = numeric_limits<extType>::lowest(),
		extType a = numeric_limits<extType>::lowest());

	void fill();

	void cleanBuffer();

	virtual GLenum getTarget() const = 0;

	virtual ~Texture();
protected:
	InternalFormat<type> format;
	void *buffer;
	ExternalFormat *buffMeta;
	GLuint texture;


	template <SamplerDataType sdt, SamplerDataType ... sdts>
	void setSamplerParametres(GLint defaultTex, const SamplerData<sdt> &sampler, const SamplerData<sdts>&...samplers);

	void setSamplerParametres(GLint defaultTex);

	virtual unsigned int linearSize() const = 0;
	virtual void load(GLenum extFormat, GLenum extType, const void *data) = 0;
	virtual GLenum getInspTarget() const = 0;

	void clean();

	Texture(Texture &&texture);
	Texture& operator=(Texture &&texture);
private:
	static const InternalFormat<type> defaultFormat;

	Texture(const Texture &texture) = delete;
	Texture& operator=(const Texture &texture) = delete;
};

/*******************************************Implementation*******************************************/

//Texture

template <>
const InternalFormat<TextureType::TT_BASE> Texture<TextureType::TT_BASE>::defaultFormat = BaseFormat<1, 1>();

template <>
const InternalFormat<TextureType::TT_INTEGRAL> Texture<TextureType::TT_INTEGRAL>::defaultFormat = IntegralFormat<1, 1>();

template <TextureType type>
Texture<type>::Texture() :
	texture(0),
	format(defaultFormat),
	buffer(nullptr),
	buffMeta(nullptr) {}

template <TextureType type>
Texture<type>::Texture(const InternalFormat<type> &_format) :
	format(_format),
	buffer(nullptr),
	buffMeta(nullptr)
{
	glGenTextures(1, &texture);
}

template <TextureType type>
Texture<type>::Texture(Texture<type> &&_texture) :
	texture(_texture.texture),
	format(_texture.format),
	buffer(_texture.buffer),
	buffMeta(_texture.buffMeta)
{
	_texture.buffer = nullptr;
	_texture.buffMeta = nullptr;
	_texture.texture = 0;
	_texture.format = defaultFormat;
}

template <TextureType type>
Texture<type>& Texture<type>::operator=(Texture<type> &&_texture)
{
	if (this != &_texture)
	{
		clean();
		texture = _texture.texture;
		format = _texture.format;
		buffer = _texture.buffer;
		buffMeta = _texture.buffMeta;

		_texture.buffer = nullptr;
		_texture.buffMeta = nullptr;
		_texture.texture = 0;
		_texture.format = defaultFormat;
	}
	return *this;
}

template <TextureType type>
template <typename extType, unsigned char _channels>
extType* Texture<type>::map()
{
	cleanBuffer();
	if (linearSize() > 0)
	{
		buffer = operator new(linearSize() * _channels * sizeof(extType));
		buffMeta = new ExtChFormatStore<extType, _channels, type>();
		GLint tmp;
		glGetIntegerv(getInspTarget(), &tmp);
		glBindTexture(getTarget(), texture);
		glGetTexImage(getTarget(), 0, ExternalFormat::format<extType, _channels, type>(), ExternalFormat::type<extType>(), buffer);
		glBindTexture(getTarget(), tmp);
	}
	return static_cast<extType*>(buffer);
}

template <TextureType type>
template <typename extType>
extType* Texture<type>::map()
{
	cleanBuffer();
	if (linearSize() > 0)
	{
		buffer = operator new(linearSize() * format.getChannels() * sizeof(extType));
		buffMeta = new ExtFormatStore<extType, type>();
		GLint tmp;
		glGetIntegerv(getInspTarget(), &tmp);
		glBindTexture(getTarget(), texture);
		glGetTexImage(getTarget(), 0, ExternalFormat::format<extType, type>(format.getChannels()), ExternalFormat::type<extType>(), buffer);
		glBindTexture(getTarget(), tmp);
	}
	return static_cast<extType*>(buffer);
}

template <TextureType type>
void Texture<type>::flush()
{
	if (buffer != nullptr)
		load(buffMeta->storedFormat(format.getChannels()), buffMeta->storedType(), buffer);
	cleanBuffer();
}

template <TextureType type>
bool Texture<type>::isDefault() const
{
	return texture == 0;
}

template <TextureType type>
void Texture<type>::clean()
{
	cleanBuffer();
	glDeleteTextures(1, &texture);
}

template <TextureType type>
template <typename extType>
void Texture<type>::fill(extType r, extType g, extType b, extType a)
{
	extType color[4] = { r, g, b, a };
	glClearTexImage(texture, 0, gl::GLenum(ExternalFormat::format<extType, 4, type>()), ExternalFormat::type<extType>(), color);
}

template <TextureType type>
void Texture<type>::fill()
{
	//TODO: ������������� �� ���������
	switch (type)
	{
	case opengl::TextureType::TT_BASE:
		fill(GLfloat(0.f), GLfloat(0.f), GLfloat(0.f), GLfloat(0.f));
		break;
	case opengl::TextureType::TT_INTEGRAL:
		fill(GLubyte(0), GLubyte(0), GLubyte(0), GLubyte(0));
		break;
	}
}

//template <>
//void Texture<TextureType::TT_BASE>::fill()
//{
//	fill(GLfloat(0.f), GLfloat(0.f), GLfloat(0.f), GLfloat(0.f));
//}
//
//template <>
//void Texture<TextureType::TT_INTEGRAL>::fill()
//{
//	fill(GLubyte(0), GLubyte(0), GLubyte(0), GLubyte(0));
//}

template <TextureType type>
template <unsigned char _channels, typename extType>
void Texture<type>::update(const extType *data)
{
	load(ExternalFormat::format<extType, _channels, type>(), ExternalFormat::type<extType>(), data);
}

template <TextureType type>
template <typename extType>
void Texture<type>::update(const extType *data)
{
	load(ExternalFormat::format<extType, type>(format.getChannels()), ExternalFormat::type<extType>(), data);
}

template <TextureType type>
void Texture<type>::update()
{
	if (buffer != nullptr)
		load(buffMeta->storedFormat(format.getChannels()), buffMeta->storedType(), buffer);
}

template <TextureType type>
const GLuint Texture<type>::getTexture() const
{
	return texture;
}

template <TextureType type>
void Texture<type>::reset()
{
	clean();
	texture = 0;
	format = defaultFormat;
}

template <TextureType type>
template <typename extType>
void Texture<type>::setNewFormat(const InternalFormat<type> &_format, extType r, extType g, extType b, extType a)
{
	format = _format;
	load(ExternalFormat::format<GLbyte, 4 , type>(), ExternalFormat::type<GLbyte>(), nullptr);
	clear(r, g, b, a);
}

template <TextureType type>
const InternalFormat<type>& Texture<type>::getFormat() const
{
	return format;
}

template <TextureType type>
unsigned int Texture<type>::size() const
{
	return linearSize() * format.getChannels();
}

template <TextureType type>
template <SamplerDataType ... sdts>
void Texture<type>::setSamplerParametres(const SamplerData<sdts>&...samplers)
{
	GLint tmp;
	glGetIntegerv(getInspTarget(), &tmp);
	glBindTexture(getTarget(), texture);
	setSamplerParametres(tmp, samplers...);
}

template <TextureType type>
template <SamplerDataType sdt, SamplerDataType ... sdts>
void Texture<type>::setSamplerParametres(GLint defaultTex, const SamplerData<sdt>& sampler, const SamplerData<sdts>&...samplers)
{
	sampler.setTexture(getTarget());
	setSamplerParametres(samplers...);
}

template <TextureType type>
void Texture<type>::setSamplerParametres(GLint defaultTex)
{
	glBindTexture(getTarget(), defaultTex);
}

template <TextureType type>
template <SamplerDataType sdt>
SamplerData<sdt> Texture<type>::getSamplerParametres() const
{
	GLint tmp;
	glGetIntegerv(getInspTarget(), &tmp);
	glBindTexture(getTarget(), texture);
	return SamplerData<dst>::fromTexture(getTarget());
	glBindTexture(getTarget(), tmp);
}

template <TextureType type>
void Texture<type>::bindTexture(unsigned int unit) const
{
	glActiveTexture(GLenum(unsigned int(GL_TEXTURE0) + unit));
	glBindTexture(getTarget(), texture);
}

template <TextureType type>
void Texture<type>::bindImage(unsigned int unit, ImageAccess acces) const
{
	glBindImageTexture(unit, texture, 0, GL_FALSE, 0, GLenum(int(acces)), format.getFormat());
}

template <TextureType type>
void Texture<type>::cleanBuffer()
{
	operator delete (buffer);
	delete buffMeta;

	buffer = nullptr;
	buffMeta = nullptr;
}

template <TextureType type>
Texture<type>::~Texture()
{
	clean();
}

}
