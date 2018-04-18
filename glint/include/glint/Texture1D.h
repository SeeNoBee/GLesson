#pragma once

#include <glint/Texture.h>
#include <iostream>

namespace opengl
{
using namespace gl;
using namespace std;

template <TextureType type>
class Texture1D : public Texture<type>
{
public:
	template <SamplerDataType...sdts>
	Texture1D(
		unsigned int width,
		const InternalFormat<type> &format = BaseFormat<>(),
		const SamplerData<sdts>& ...samplers);

	//TODO: constructors

	Texture1D();

	Texture1D(Texture1D &&texture);
	Texture1D& operator=(Texture1D &&texture);

	unsigned int getWidth() const;

	template<typename extType>
	void resize(unsigned int newWidth, extType r = extType(0), extType g = extType(0), extType b = extType(0), extType a = extType(1));

	template<unsigned char channels, typename extType>
	void resize(unsigned int newWidth, const extType *data);

	template<typename extType>
	void resize(unsigned int newWidth, const extType *data);

	void reset() override;
	GLenum getTarget() const override;

	~Texture1D();
protected:
	unsigned int linearSize() const override;
	void load(GLenum extFormat, GLenum extType, const void *data) override;
	GLenum getInspTarget() const override;
private:
	unsigned int width; //TODO: ����� �� �������, � ��������� �� ��������

	Texture1D(const Texture1D &texture) = delete;
	Texture1D& operator=(const Texture1D &source) = delete;
};

typedef Texture1D<TT_BASE> Texture1;
typedef Texture1D<TT_INTEGRAL> Texture1i;

/*******************************************Implementation*******************************************/

template <TextureType type>
template <SamplerDataType...sdts>
Texture1D<type>::Texture1D(
	unsigned int _width,
	const InternalFormat<type> &_format,
	const SamplerData<sdts>& ...samplers) :
	Texture(_format),
	width(_width)
{
	GLint tmp;
	glGetIntegerv(GL_TEXTURE_BINDING_1D, &tmp);
	glBindTexture(GL_TEXTURE_1D, texture);
	glTexImage1D(GL_TEXTURE_1D, 0, format.getFormat(), width, 0, ExternalFormat::format<GLbyte, 4, type>(), ExternalFormat::type<GLbyte>(), this);

	fill();

	setSamplerParametres(tmp, samplers...);
}

template <TextureType type>
Texture1D<type>::Texture1D() :
	Texture<type>(),
	width(0) {}

template <TextureType type>
Texture1D<type>::Texture1D(Texture1D<type> &&_texture) :
	Texture<type>(move(_texture))
{
	width = _texture.width;
	_texture.width = 0;
}

template <TextureType type>
Texture1D<type>& Texture1D<type>::operator=(Texture1D<type> &&_texture)
{
	Texture<type>::operator=(move(_texture));
	width = _texture.width;
	_texture.width = 0;
	return *this;
}

template <TextureType type>
void Texture1D<type>::load(GLenum extFormat, GLenum extType, const void *data)
{
	GLint tmp;
	glGetIntegerv(GL_TEXTURE_BINDING_1D, &tmp);
	glBindTexture(GL_TEXTURE_1D, texture);
	glTexImage1D(GL_TEXTURE_1D, 0, format.getFormat(), width, 0, extFormat, extType, data);
	glBindTexture(GL_TEXTURE_1D, tmp);
}

template <TextureType type>
GLenum Texture1D<type>::getInspTarget() const
{
	return GL_TEXTURE_BINDING_1D;
}

template <TextureType type>
unsigned int Texture1D<type>::getWidth() const
{
	return width;
}

template <TextureType type>
void Texture1D<type>::reset()
{
	width = 0;
	Texture<type>::reset();
}

template <TextureType type>
GLenum Texture1D<type>::getTarget() const
{
	return GL_TEXTURE_1D;
}

template <TextureType type>
template<typename extType>
void Texture1D<type>::resize(unsigned int newWidth, extType r, extType g, extType b, extType a)
{
	width = newWidth;
	GLint tmp;
	glGetIntegerv(GL_TEXTURE_BINDING_1D, &tmp);
	glBindTexture(GL_TEXTURE_1D, texture);
	glTexImage1D(GL_TEXTURE_1D, 0, format.getFormat(), width, 0, ExternalFormat::format<extType, 4, type>(), ExternalFormat::type<extType>(), nullptr);
	glBindTexture(GL_TEXTURE_1D, tmp);
	fill(r, g, b, a);
}

template <TextureType type>
template<typename extType>
void Texture1D<type>::resize(unsigned int newWidth, const extType *data)
{
	width = newWidth;
	update(data);
}

template <TextureType type>
template<unsigned char _channels, typename extType>
void Texture1D<type>::resize(unsigned int newWidth, const extType *data)
{
	width = newWidth;
	update<_channels>(data);
}

template <TextureType type>
unsigned int Texture1D<type>::linearSize() const
{
	return width;
}

template <TextureType type>
Texture1D<type>::~Texture1D() {}

}
