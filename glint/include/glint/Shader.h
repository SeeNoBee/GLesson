#pragma once

#include <glbinding\gl\gl.h>
#include <sstream>

namespace opengl
{
using namespace gl;
using namespace std;

constexpr unsigned char unitNumber(unsigned char number)
{
	return (number & 1) +
		((number >> 1) & 1) +
		((number >> 2) & 1) +
		((number >> 3) & 1) +
		((number >> 4) & 1) +
		((number >> 5) & 1) +
		((number >> 6) & 1) +
		((number >> 7) & 1);
}

//TODO: доделать
//TODO: нельзя инициализировать инлайново внутри класса статическое аоле некостантаными и "неинтеджеровыми" типы
//#define ENUM_NAME ShaderType
//#define ENUM_PREDICATE SH
//#define ENUM_OBJECT \
//BEGIN_ENUM(unsigned char)\
//	ENUM_MEMBER(VERTEX, Vertex shader type)\
//	ENUM_MEMBER(FRAGMENT, Fragment shader type)\
//	ENUM_MEMBER(TESSELATION_CONTROL, Tesselation control shader type)\
//	ENUM_MEMBER(TESSELATION_EVALUATION, Tesselation evaluation shader type)\
//	ENUM_MEMBER(GEOMETRY, Geometry shader type)\
//	ENUM_MEMBER(COMPUTE, Compute shader type)\
//END_ENUM
//#define ENUM_CONSTANTS
//#include <EnumTool.h>

enum class ShaderType : unsigned char
{
	SH_VERTEX = 0,
	SH_FRAGMENT,
	SH_TESSELATION_CONTROL,
	SH_TESSELATION_EVALUATION,
	SH_GEOMETRY,
	SH_COMPUTE
};

//TODO: это тоже загнать под макрос
static const ShaderType SH_VERTEX = ShaderType::SH_VERTEX;
static const ShaderType SH_FRAGMENT = ShaderType::SH_FRAGMENT;
static const ShaderType SH_TESSELATION_CONTROL = ShaderType::SH_TESSELATION_CONTROL;
static const ShaderType SH_TESSELATION_EVALUATION = ShaderType::SH_TESSELATION_EVALUATION;
static const ShaderType SH_GEOMETRY = ShaderType::SH_GEOMETRY;
static const ShaderType SH_COMPUTE = ShaderType::SH_COMPUTE;

class ShaderObject
{
public:
	ShaderObject(ShaderType type, const string &source, const string &prefix = "");
	ShaderObject(ShaderType type); 
	ShaderObject(ShaderType type, const initializer_list<string> &paths, const string &prefix = "");

	ShaderObject(ShaderObject &&source);
	//TODO: на самом деле есть функция getShaderSource => можно не только перемещать шейдеры, но и копировать
	ShaderObject& operator=(ShaderObject &&source);

	const GLuint getShader() const;
	ShaderType getType() const;
	string collectLog() const;
	unsigned char getErrorFlags() const;
	bool compileStatus() const;
	bool isCreated() const;
	bool noError() const;
	string getCompileLog() const;
	void reset();

	~ShaderObject();
private:

	enum ShaderErrorFlags : unsigned char
	{
		SH_ERR_NO_FILE = 1,
		SH_ERR_USING_STANDART = 2,
		SH_ERR_FAILED_CREATE = 4,
		SH_ERR_FAILED_COMPILE = 8,
		SH_ERR_NUMBER = 4
	};

	static const GLenum glShaderTypes[6];
	static const string shaderTypeExtentions[6];
	static const string stdSources[6];
	static const string shaderErrorNames[SH_ERR_NUMBER];

	template <unsigned char ShType>
	friend class RenderShaderProgram;

	GLuint shader;
	ShaderType type;
	unsigned char errorLogFlags;

	ShaderObject();
	ShaderObject(const ShaderObject &shader) = delete;
	ShaderObject & operator=(const ShaderObject &shader) = delete;

	bool loadFromFile(const string &path, ostringstream &dst);
	void create(const string &source);
};

template <ShaderType Type>
class Shader
{
public:
	Shader();
	Shader(const string &source, const string &prefix = "");
	Shader(const initializer_list<string> &paths, const string &prefix = "");
	Shader(ShaderObject &&shader);

	const ShaderObject& getShaderObject() const;
private:
	template <unsigned char ShType>
	friend class RenderShaderProgram;

	ShaderObject shader;

	Shader(const Shader &shader) = delete;
	Shader& operator=(const Shader &shader) = delete;
};

/*******************************************Implementation*******************************************/

//Shader
template<ShaderType type>
Shader<type>::Shader() :
	shader(ShaderObject(type))
{}

template<ShaderType type>
Shader<type>::Shader(const string &source, const string &prefix) :
	shader(ShaderObject(type, source, prefix))
{}

template<ShaderType type>
Shader<type>::Shader(const initializer_list<string> &paths, const string &prefix) :
	shader(ShaderObject(type, paths, prefix))
{}

template<ShaderType type>
Shader<type>::Shader(ShaderObject &&_shader) :
	shader((_shader.getType() == type) ? move(_shader) : ShaderObject(type))
{}

template<ShaderType type>
const ShaderObject& Shader<type>::getShaderObject() const
{
	return shader;
}

}