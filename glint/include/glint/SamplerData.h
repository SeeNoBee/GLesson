#pragma once

#include <glbinding\gl\gl.h>

namespace opengl
{

using namespace gl;

//TODO: ������� static const � ������ �� �������� ����������
//TODO: ��� �������� �������, �������� ��� ����� ���������� ����� ���������

//TODO: �������� ����� �����
enum SamplerDataType : unsigned char
{
	SDT_WRAP,
	SDT_FILTER,
	SDT_LOD,
	SDT_COMPARE,
	SDT_BORDER,
	//TODO: max anisotropy
	SDT_DATA_TYPE_NUM
};

template <SamplerDataType type>
struct SamplerData
{
	SamplerData()
	{
		static_assert(false, "undefined sampler data type");
	}
	void setTexture(GLenum target) const
	{
		static_assert(false, "undefined sampler data type")
	}
	void setSampler(GLuint sampler) const
	{
		static_assert(false, "undefined sampler data type");
	}

	static SamplerData fromTexture(GLenum target)
	{
		static_assert(false, "undefined sampler data type");
	}
	static SamplerData fromSampler(GLuint sampler)
	{
		static_assert(false, "undefined sampler data type");
	}
};

enum class WrapMode
{
	WM_REPEAT = int(GL_REPEAT),
	WM_MIRRORED_REPEAT = int(GL_MIRRORED_REPEAT),
	WM_CLAMP_TO_EDGE = int(GL_CLAMP_TO_EDGE),
	WM_CLAMP_TO_BORDER = int(GL_CLAMP_TO_BORDER)
};

static const WrapMode WM_REPEAT = WrapMode::WM_REPEAT;
static const WrapMode WM_MIRRORED_REPEAT = WrapMode::WM_MIRRORED_REPEAT;
static const WrapMode WM_CLAMP_TO_EDGE = WrapMode::WM_CLAMP_TO_EDGE;
static const WrapMode WM_CLAMP_TO_BORDER = WrapMode::WM_CLAMP_TO_BORDER;

template<>
struct SamplerData<SDT_WRAP>
{
	WrapMode wrapS;
	WrapMode wrapT;
	WrapMode wrapR;

	SamplerData(
		WrapMode S = WrapMode::WM_REPEAT,
		WrapMode T = WrapMode::WM_REPEAT,
		WrapMode R = WrapMode::WM_REPEAT):
		wrapS(S),
		wrapT(T),
		wrapR(R) {}

	void setTexture(GLenum target) const
	{
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GLint(wrapS));
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GLint(wrapT));
		glTexParameteri(target, GL_TEXTURE_WRAP_R, GLint(wrapR));
	}
	void setSampler(GLuint sampler) const
	{
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_S, GLuint(wrapS));
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_T, GLuint(wrapT));
		glSamplerParameteri(sampler, GL_TEXTURE_WRAP_R, GLuint(wrapR));
	}

	static SamplerData fromTexture(GLenum target)
	{
		GLint tmp[3];
		glGetTexParameteriv(target, GL_TEXTURE_WRAP_S, tmp);
		glGetTexParameteriv(target, GL_TEXTURE_WRAP_T, tmp + 1);
		glGetTexParameteriv(target, GL_TEXTURE_WRAP_R, tmp + 2);

		return SamplerData(WrapMode(tmp[0]), WrapMode(tmp[1]), WrapMode(tmp[2]));
	}
	static SamplerData fromSampler(GLuint sampler)
	{
		GLint wraps[3];
		glGetSamplerParameteriv(sampler, GL_TEXTURE_WRAP_S, wraps);
		glGetSamplerParameteriv(sampler, GL_TEXTURE_WRAP_T, wraps + 1);
		glGetSamplerParameteriv(sampler, GL_TEXTURE_WRAP_R, wraps + 2);

		return SamplerData(WrapMode(wraps[0]), WrapMode(wraps[1]), WrapMode(wraps[2]));

	}
};

enum class FilterMode
{
	FM_NEAREST = int(GL_NEAREST),
	FM_LINEAR = int(GL_LINEAR)
	//TODO: ���� ���, ��������� ������
};

static const FilterMode FM_NEAREST = FilterMode::FM_NEAREST;
static const FilterMode FM_LINEAR = FilterMode::FM_LINEAR;


template <>
struct SamplerData<SDT_FILTER>
{
	FilterMode minFilter;
	FilterMode magFilter;

	SamplerData(
		FilterMode min = FilterMode::FM_LINEAR,
		FilterMode mag = FilterMode::FM_LINEAR) :
		minFilter(min),
		magFilter(mag) {}

	void setTexture(GLenum target) const
	{
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GLint(minFilter));
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GLint(magFilter));
	}
	void setSampler(GLuint sampler) const
	{
		glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GLint(minFilter));
		glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GLint(magFilter));
	}

	static SamplerData fromTexture(GLenum target)
	{
		GLint tmp[2];

		glGetTexParameteriv(target, GL_TEXTURE_MIN_FILTER, tmp);
		glGetTexParameteriv(target, GL_TEXTURE_MAG_FILTER, tmp + 1);

		return SamplerData(FilterMode(tmp[0]), FilterMode(tmp[1]));
	}
	static SamplerData fromSampler(GLuint sampler)
	{
		GLint filters[2];

		glGetSamplerParameteriv(sampler, GL_TEXTURE_MIN_FILTER, filters);
		glGetSamplerParameteriv(sampler, GL_TEXTURE_MAG_FILTER, filters + 1);

		return SamplerData(FilterMode(filters[0]), FilterMode(filters[1]));
	}
};


template <>
struct SamplerData<SDT_LOD>
{
	GLfloat minLOD;
	GLfloat maxLOD;
	GLfloat biasLOD;

	SamplerData(
		GLfloat min = -1000.f,
		GLfloat max = 1000.f,
		GLfloat bias = 0.f) :
		minLOD(min),
		maxLOD(max),
		biasLOD(bias) {}

	void setTexture(GLenum target) const
	{
		glTexParameterf(target, GL_TEXTURE_MIN_LOD, minLOD);
		glTexParameterf(target, GL_TEXTURE_MAX_LOD, maxLOD);
		glTexParameterf(target, GL_TEXTURE_LOD_BIAS, biasLOD);
	}

	void setSampler(GLuint sampler) const
	{
		glSamplerParameterf(sampler, GL_TEXTURE_MIN_LOD, minLOD);
		glSamplerParameterf(sampler, GL_TEXTURE_MAX_LOD, maxLOD);
		glSamplerParameterf(sampler, GL_TEXTURE_LOD_BIAS, biasLOD);
	}

	static SamplerData fromTexture(GLenum target)
	{
		GLfloat tmp[3];
		glGetTexParameterfv(target, GL_TEXTURE_MIN_LOD, tmp);
		glGetTexParameterfv(target, GL_TEXTURE_MAX_LOD, tmp + 1);
		glGetTexParameterfv(target, GL_TEXTURE_LOD_BIAS, tmp + 2);

		return SamplerData(tmp[0], tmp[1], tmp[3]);

	}

	static SamplerData fromSampler(GLuint sampler)
	{
		GLfloat tmp[3];
		glGetSamplerParameterfv(sampler, GL_TEXTURE_MIN_LOD, tmp);
		glGetSamplerParameterfv(sampler, GL_TEXTURE_MAX_LOD, tmp + 1);
		glGetSamplerParameterfv(sampler, GL_TEXTURE_LOD_BIAS, tmp + 2);

		return SamplerData(tmp[0], tmp[1], tmp[3]);
	}
};

enum class TextureCompareMode
{
	TCM_NONE = int(GL_NONE),
	TCM_COMPARE_REF_TO_TEXTURE = int(GL_COMPARE_REF_TO_TEXTURE)
};

static const TextureCompareMode TCM_NONE = TextureCompareMode::TCM_NONE;
static const TextureCompareMode TCM_COMPARE_REF_TO_TEXTURE = TextureCompareMode::TCM_COMPARE_REF_TO_TEXTURE;

enum class TextureCompareFunc
{
	TCF_NEVER = int(GL_NEVER),
	TCF_ALWAYS = int(GL_ALWAYS),
	TCF_NOTEQUAL = int(GL_NOTEQUAL),
	TCF_EQUAL = int(GL_EQUAL),
	TCF_GREATHER = int(GL_GREATER),
	TCF_LESS = int(GL_LESS),
	TCF_GEQUAL = int(GL_GEQUAL),
	TCF_LEQUAL = int(GL_LEQUAL)
};

static const TextureCompareFunc TCF_NEVER = TextureCompareFunc::TCF_NEVER;
static const TextureCompareFunc TCF_ALWAYS = TextureCompareFunc::TCF_ALWAYS;
static const TextureCompareFunc TCF_NOTEQUAL = TextureCompareFunc::TCF_NOTEQUAL;
static const TextureCompareFunc TCF_EQUAL = TextureCompareFunc::TCF_EQUAL;
static const TextureCompareFunc TCF_GREATHER = TextureCompareFunc::TCF_GREATHER;
static const TextureCompareFunc TCF_LESS = TextureCompareFunc::TCF_LESS;
static const TextureCompareFunc TCF_GEQUAL = TextureCompareFunc::TCF_GEQUAL;
static const TextureCompareFunc TCF_LEQUAL = TextureCompareFunc::TCF_LEQUAL;

template <>
struct SamplerData<SDT_COMPARE>
{
	TextureCompareMode compareMode;
	TextureCompareFunc compareFunc;

	SamplerData(
		TextureCompareMode mode = TextureCompareMode::TCM_COMPARE_REF_TO_TEXTURE,
		TextureCompareFunc func = TextureCompareFunc::TCF_LEQUAL):
		compareMode(mode),
		compareFunc(func) {}

	void setTexture(GLenum target) const
	{
		glTexParameteri(target, GL_TEXTURE_COMPARE_MODE, GLint(compareMode));
		glTexParameteri(target, GL_TEXTURE_COMPARE_FUNC, GLint(compareFunc));
	}
	void setSampler(GLuint sampler) const
	{
		glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_MODE, GLint(compareMode));
		glSamplerParameteri(sampler, GL_TEXTURE_COMPARE_FUNC, GLint(compareFunc));
	}

	static SamplerData fromTexture(GLenum target)
	{
		GLint tmp[2];
		glGetTexParameteriv(target, GL_TEXTURE_COMPARE_MODE, tmp);
		glGetTexParameteriv(target, GL_TEXTURE_COMPARE_FUNC, tmp + 1);

		return SamplerData(TextureCompareMode(tmp[0]), TextureCompareFunc(tmp[1]));

	}

	static SamplerData fromSampler(GLuint sampler)
	{
		GLint tmp[2];
		glGetSamplerParameteriv(sampler, GL_TEXTURE_COMPARE_MODE, tmp);
		glGetSamplerParameteriv(sampler, GL_TEXTURE_COMPARE_FUNC, tmp + 1);

		return SamplerData(TextureCompareMode(tmp[0]), TextureCompareFunc(tmp[1]));
	}
};

template <>
struct SamplerData<SDT_BORDER>
{
	vec4 color;

	SamplerData(const vec4 &_color = vec4(0.f, 0.f, 0.f, 1.f)) :
		color(_color) {}
	void setTexture(GLenum target) const
	{
		GLfloat tmp[4] = { color[0], color[1], color[2], color[3] };
		glTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, tmp);
	}

	void setSampler(GLuint sampler) const
	{
		GLfloat tmp[4] = { color[0], color[1], color[2], color[3] };
		glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, tmp);
	}

	static SamplerData fromTexture(GLenum target)
	{
		GLfloat tmp[4];
		glGetTexParameterfv(target, GL_TEXTURE_BORDER_COLOR, tmp);

		return SamplerData(vec4(tmp));
	}

	static SamplerData fromSampler(GLuint sampler)
	{
		GLfloat tmp[4];
		glGetSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, tmp);

		return SamplerData(vec4(tmp));
	}
};

}
