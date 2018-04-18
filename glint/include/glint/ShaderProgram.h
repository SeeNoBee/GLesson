#pragma once

#include <glint/Shader.h>
#include <glint/Uniform.h>

namespace opengl
{

enum RenderShaderProgramFlags : unsigned char
{
	SHF_VERTEX = 1,
	SHF_FRAGMENT = 2,
	SHF_TESSELATION_CONTROL = 4,
	SHF_TESSELATION_EVALUATION = 8,
	SHF_GEOMETRY = 16,
	SHF_STD = 1 | 2
};

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(ShaderProgram &&shaderProgram);
	ShaderProgram& operator=(ShaderProgram &&shaderProgram);

	void use() const;
	virtual string collectLog() const;
	void logToFile(const string &path = "shaderprogram.log") const;
	const gl::GLuint getProgram() const;
	unsigned char getErrorFlags() const;
	bool isCreated() const;
	bool linkStatus() const;
	bool shaderHealthStatus() const;
	bool noErrors() const;
	string getLinkLog() const;

	gl::GLint findUniform(const char* name) const;
	bool check(const char * name) const;

	template <class type>
	void uniform(const char *name, const type &value);

	template <class type>
	void uniform(GLint location, const type &value);

	template <class type>
	type uniform(const char *name);

	template <class type>
	type uniform(GLint location);

	virtual ~ShaderProgram() = 0;
protected:
	enum ShaderProgramErrorFlags : unsigned char
	{
		SH_PR_ERR_FAILED_CREATE = 1,
		SH_PR_ERR_FAILED_LINK = 2,
		SH_PR_ERR_SHADER_ERR = 4,
		SH_PR_ERR_NUMBER = 3
	};

	static string shaderProgramErrorNames[SH_PR_ERR_NUMBER];

	GLuint program;
	unsigned char errorLogFlags;
private:
	ShaderProgram(const ShaderProgram &shaderProgram) = delete;
	ShaderProgram& operator=(const ShaderProgram &shaderProgram) = delete;
};

template <unsigned char shader_set>
class RenderShaderProgram : public ShaderProgram
{
	static_assert((shader_set & SHF_STD) == SHF_STD, "'vert' and 'frag' shaders are required for noncompute shader");
	static_assert(shader_set <= 31, "invalid shader set: only 'vert', 'frag', 'tesc', 'tese', 'geom' are avaliable for render");
public:
	RenderShaderProgram(RenderShaderProgram &&renderProgram);
	RenderShaderProgram& operator=(RenderShaderProgram &&renderProgram);

	template <ShaderType ...types>
	RenderShaderProgram(Shader<types>&&... shaders);

	RenderShaderProgram(const string &name);

	string collectLog() const override;

	template <ShaderType type>
	const ShaderObject& getShader() const;


	~RenderShaderProgram();
private:
	//TODO: ��-�� ����� ������ ����� ���������� (�1001), �� ����� ���-�� �� ���������, ��� ����� ���������� �� ���������
	//template <ShaderType ...types>
	//RenderShaderProgram(Shader<types>&... args) = delete;


	ShaderObject shaders[unitNumber(shader_set)];
	//TODO: unsigned short int convPositions;

	template<unsigned char used_log, ShaderType type, ShaderType ...types>
	void create(Shader<type> &&shader, Shader<types>&&... shaders);

	template<unsigned char used_log>
	void create();
};

//TODO: ComputeShaderProgram

/*******************************************Implementation*******************************************/
//ShaderProgram
template <class type>
void ShaderProgram::uniform(const char *name, const type &value)
{
	Uniform<type>::set(findUniform(name), value);
}

template <class type>
void ShaderProgram::uniform(GLint location, const type &value)
{
	Uniform<type>::set(location, value);
}

template <class type>
type ShaderProgram::uniform(const char *name)
{
	return Uniform<type>::get(findUniform(name), program);
}

template <class type>
type ShaderProgram::uniform(GLint location)
{
	return Uniform<type>::get(location, program);
}

//RenderShaderProgram
template <unsigned char shader_set>
RenderShaderProgram<shader_set>::RenderShaderProgram(const string &name)
{
	unsigned char index = 0;
	for (unsigned char i = 0; i < 5; ++i)
		if ((shader_set >> i) & 1)
		{
			shaders[index] = ShaderObject(ShaderType(i), name + shaderTypeExtentions[i]);
			++index;
		}
	create<shader_set>();
}

template <unsigned char shader_set>
RenderShaderProgram<shader_set>::RenderShaderProgram(RenderShaderProgram<shader_set> &&renderProgram):
	ShaderProgram(move(renderProgram))
{
	for (unsigned char i = 0; i < unitNumber(shader_set); ++i)
		shaders[i] = move(renderProgram.shaders[i]);
	renderProgram.errorLogFlags |= SH_PR_ERR_SHADER_ERR;
}

template <unsigned char shader_set>
RenderShaderProgram<shader_set>& RenderShaderProgram<shader_set>::operator=(RenderShaderProgram<shader_set> &&renderProgram)
{
	if (this != &renderProgram)
	{
		for (unsigned char i = 0; i < unitNumber(shader_set); ++i)
		{
			glDetachShader(program, shaders[i].shader);
			shaders[i] = move(renderProgram.shaders[i]);
		}
		ShaderProgram::operator=(move(renderProgram));
		renderProgram.errorLogFlags |= SH_PR_ERR_SHADER_ERR;
	}
	return *this;
}

template <unsigned char shader_set>
template <ShaderType ...types>
RenderShaderProgram<shader_set>::RenderShaderProgram(Shader<types>&&... shaders)
{
	if (isCreated())
		create<0>(move(shaders)...);
}

template <unsigned char shader_set>
template<unsigned char used_log, ShaderType type, ShaderType ...types>
void RenderShaderProgram<shader_set>::create(Shader<type> &&shader, Shader<types>&&... shaders)
{
	static_assert((sizeof...(types)) <= (unitNumber(shader_set) - 1), "too much shaders");
	static_assert(((shader_set & (1 << unsigned char(type))) != 0), "invalid shader type for this shader program");
	static_assert(((used_log & (1 << unsigned char(type))) == 0), "repeat the shader type");

	shaders[sizeof...(types)] = move(shader.shader);

	create<used_log | (1 << unsigned char(type))>(move(shaders)...);
}

template <unsigned char shader_set>
template<unsigned char used_log>
void RenderShaderProgram<shader_set>::create()
{
	static_assert(used_log == shader_set, "some shaders are undefined");

	for (unsigned char i = 0; i < unitNumber(shader_set); ++i)
	{
		if (!shaders[i].noError())
			errorLogFlags |= SH_PR_ERR_SHADER_ERR;
		glAttachShader(program, shaders[i].shader);
	}

	glLinkProgram(program);

	GLint linkStatus;

	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus == GLint(GL_FALSE))
		errorLogFlags |= SH_PR_ERR_FAILED_LINK;
}

template <unsigned char shader_set>
string RenderShaderProgram<shader_set>::collectLog() const
{
	string tmp(ShaderProgram::collectLog() + '\n');
	for (unsigned char i = 0; i < unitNumber(shader_set); ++i)
		if (!shaders[i].noError())
			tmp += shaders[i].collectLog();
	return tmp;
}

template <unsigned char shader_set>
RenderShaderProgram<shader_set>::~RenderShaderProgram()
{
	if (isCreated())
	{
		for (unsigned char i = 0; i < unitNumber(shader_set); ++i)
			glDetachShader(program, shaders[i].shader);
		glDeleteProgram(program);
	}
}

}
