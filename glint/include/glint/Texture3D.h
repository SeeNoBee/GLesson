#pragma once

#include <glint/Texture.h>

namespace opengl
{
using namespace gl;

template <TextureType type>
class Texture3D : public Texture<type>
{
public:
	template <SamplerDataType...sdts>
	Texture3D(
		unsigned int width,
		unsigned int height,
		unsigned int depth,
		const InternalFormat<type> &format = BaseFormat<>(),
		const SamplerData<sdts>& ...samplers);

	//TODO: constructors

	Texture3D();

	Texture3D(Texture3D &&texture);
	Texture3D& operator=(Texture3D &&texture);

	unsigned int getWidth() const;
	unsigned int getHeight() const;
	unsigned int getDepth() const;

	template<typename extType>
	void resize(unsigned int newWidth, unsigned int newHeight, unsigned int newDepth, extType r = extType(0), extType g = extType(0), extType b = extType(0), extType a = extType(1));

	template<unsigned char channels, typename extType>
	void resize(unsigned int newWidth, unsigned int newHeight, unsigned int newDepth, const extType *data);

	template<typename extType>
	void resize(unsigned int newWidth, unsigned int newHeight, unsigned int newDepth, const extType *data);

	void reset() override;
	GLenum getTarget() const override;

	~Texture3D();
protected:
	unsigned int linearSize() const override;
	void load(GLenum extFormat, GLenum extType, const void *data) override;
	GLenum getInspTarget() const override;
private:
	unsigned int width; //TODO: ����� �� �������, � ��������� �� ��������
	unsigned int height; //TODO: ����� �� �������, � ��������� �� ��������
	unsigned int depth; //TODO: ����� �� �������, � ��������� �� ��������

	Texture3D(const Texture3D &texture) = delete;
	Texture3D& operator=(const Texture3D &source) = delete;
};

typedef Texture3D<TT_BASE> Texture3;
typedef Texture3D<TT_INTEGRAL> Texture3i;

/*******************************************Implementation*******************************************/

template <TextureType type>
template <SamplerDataType...sdts>
Texture3D<type>::Texture3D(
	unsigned int _width,
	unsigned int _height,
	unsigned int _depth,
	const InternalFormat<type> &_format,
	const SamplerData<sdts>& ...samplers) :
	Texture(_format),
	width(_width),
	height(_height),
	depth(_depth)
{
	GLint tmp;
	glGetIntegerv(GL_TEXTURE_BINDING_3D, &tmp);
	glBindTexture(GL_TEXTURE_3D, texture);
	glTexImage3D(GL_TEXTURE_3D, 0, format.getFormat(), width, height, depth, 0, ExternalFormat::format<GLbyte, 4, type>(), ExternalFormat::type<GLbyte>(), nullptr);

	fill();

	setSamplerParametres(tmp, samplers...);
}

template <TextureType type>
Texture3D<type>::Texture3D() :
	Texture<type>(),
	width(0),
	height(0),
	depth(0) {}

template <TextureType type>
Texture3D<type>::Texture3D(Texture3D<type> &&_texture) :
	Texture<type>(move(_texture))
{
	width = _texture.width;
	height = _texture.height;
	depth = _texture.depth;
	_texture.width = 0;
	_texture.height = 0;
	_texture.depth = 0;
}

template <TextureType type>
Texture3D<type>& Texture3D<type>::operator=(Texture3D<type> &&_texture)
{
	Texture<type>::operator=(move(_texture));
	width = _texture.width;
	height = _texture.height;
	depth = _texture.depth;
	_texture.width = 0;
	_texture.height = 0;
	_texture.depth = 0;
	return *this;
}

template <TextureType type>
void Texture3D<type>::load(GLenum extFormat, GLenum extType, const void *data)
{
	GLint tmp;
	glGetIntegerv(GL_TEXTURE_BINDING_3D, &tmp);
	glBindTexture(GL_TEXTURE_3D, texture);
	glTexImage3D(GL_TEXTURE_3D, 0, format.getFormat(), width, height, depth, 0, extFormat, extType, data);
	glBindTexture(GL_TEXTURE_3D, tmp);
}

template <TextureType type>
GLenum Texture3D<type>::getInspTarget() const
{
	return GL_TEXTURE_BINDING_3D;
}

template <TextureType type>
unsigned int Texture3D<type>::getWidth() const
{
	return width;
}

template <TextureType type>
unsigned int Texture3D<type>::getHeight() const
{
	return height;
}

template <TextureType type>
unsigned int Texture3D<type>::getDepth() const
{
	return depth;
}

template <TextureType type>
void Texture3D<type>::reset()
{
	width = 0;
	height = 0;
	depth = 0;
	Texture<type>::reset();
}

template <TextureType type>
GLenum Texture3D<type>::getTarget() const
{
	return GL_TEXTURE_3D;
}

template <TextureType type>
template<typename extType>
void Texture3D<type>::resize(unsigned int newWidth, unsigned int newHeight, unsigned int newDepth, extType r, extType g, extType b, extType a)
{
	width = newWidth;
	height = newHeight;
	depth = newDepth;
	GLint tmp;
	glGetIntegerv(GL_TEXTURE_BINDING_3D, &tmp);
	glBindTexture(GL_TEXTURE_3D, texture);
	glTexImage3D(GL_TEXTURE_3D, 0, format.getFormat(), width, height, depth, 0, ExternalFormat::format<extType, 4, type>(), ExternalFormat::type<extType>(), nullptr);
	glBindTexture(GL_TEXTURE_3D, tmp);
	fill(r, g, b, a);
}

template <TextureType type>
template<typename extType>
void Texture3D<type>::resize(unsigned int newWidth, unsigned int newHeight, unsigned int newDepth, const extType *data)
{
	width = newWidth;
	height = newHeight;
	depth = newDepth;
	update(data);
}

template <TextureType type>
template<unsigned char _channels, typename extType>
void Texture3D<type>::resize(unsigned int newWidth, unsigned int newHeight, unsigned int newDepth, const extType *data)
{
	width = newWidth;
	height = newHeight;
	depth = newDepth;
	update<_channels>(data);
}

template <TextureType type>
unsigned int Texture3D<type>::linearSize() const
{
	return width * height * depth;
}

template <TextureType type>
Texture3D<type>::~Texture3D() {}
}
