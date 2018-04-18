#include <glint/ShaderProgram.h>
#include <fstream>
#include <sstream>

namespace opengl
{

using namespace gl;
using namespace std;

//ShaderProgram
ShaderProgram::ShaderProgram() :
	errorLogFlags(0),
	program(glCreateProgram())
{
	if (program == 0)
		errorLogFlags |= SH_PR_ERR_FAILED_CREATE;
}

ShaderProgram::ShaderProgram(ShaderProgram &&shaderProgram):
	program(shaderProgram.program),
	errorLogFlags(shaderProgram.errorLogFlags)
{
	shaderProgram.program = 0;
	shaderProgram.errorLogFlags = SH_PR_ERR_FAILED_CREATE;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram &&shaderProgram)
{
	if (this != &shaderProgram)
	{
		glDeleteProgram(program);
		program = shaderProgram.program;
		errorLogFlags = shaderProgram.errorLogFlags;

		shaderProgram.program = 0;
		shaderProgram.errorLogFlags = SH_PR_ERR_FAILED_CREATE;
	}
	return *this;
}

void ShaderProgram::use() const
{
	glUseProgram(program);
}

const GLuint ShaderProgram::getProgram() const
{
	return program;
}

unsigned char ShaderProgram::getErrorFlags() const
{
	return errorLogFlags;
}

bool ShaderProgram::isCreated() const
{
	return ((errorLogFlags & SH_PR_ERR_FAILED_CREATE) == 0);
}

bool ShaderProgram::linkStatus() const
{
	return ((errorLogFlags & SH_PR_ERR_FAILED_LINK) == 0);
}

bool ShaderProgram::noErrors() const
{
	return (errorLogFlags == 0);
}

bool ShaderProgram::shaderHealthStatus() const
{
	return ((errorLogFlags & SH_PR_ERR_SHADER_ERR) == 0);
}

string ShaderProgram::getLinkLog() const
{
	if (!isCreated())
		return "link log is not available for uncreated shader program\n";

	string tmp;

	glValidateProgram(program); //TODO: ��� �� ����� ���� �� ��� ��������, � ����� ���������, ��� ��� ������ �������� ��������, � ��� �������� ������
	GLint logSize = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);

	if (logSize > 0)
	{
		GLchar *log = new GLchar[logSize];
		glGetProgramInfoLog(program, logSize, nullptr, log);

		tmp.reserve(logSize);
		tmp.assign(log);
		delete [] log;
	}

	return tmp;
}

string ShaderProgram::collectLog() const
{
	if (errorLogFlags == 0)
		return "";
	ostringstream log;
	log << "shader program log:" << endl;
	for (unsigned char i = 0; i < SH_PR_ERR_NUMBER; ++i)
		if ((errorLogFlags >> i) & 1)
			log << ' ' << shaderProgramErrorNames[i] << endl;
	if (!linkStatus())
		log << ">>>>>>>>>>link log<<<<<<<<<<" << endl << getLinkLog() + ">>>>>>>>>>>>>><<<<<<<<<<<<<<" << endl;
	return log.str();
}

void ShaderProgram::logToFile(const string &path) const
{
	ofstream file;
	file.open(path.c_str(), ios::out);

	if (file.is_open())
		file << collectLog();

	file.close();
}

GLint ShaderProgram::findUniform(const char* name) const
{
	return glGetUniformLocation(program, name);
}

bool ShaderProgram::check(const char * name) const
{
	return findUniform(name) != -1;
}

//RenderDhaderProgram
template <unsigned char shader_set>
template <ShaderType type>
const ShaderObject& RenderShaderProgram<shader_set>::getShader() const
{
	static_assert((shader_set & (1 << unsigned char(type))) != 0, "there is no such shader type in this shader set");

	//TODO: ������������, ���������� �� �������
	for (unsigned char i = 0; i < unitNumber(shader_set); ++i)
		if (type == shaders[i].type)
			return shaders[i];
}

ShaderProgram::~ShaderProgram()
{
	if (isCreated())
		glDeleteProgram(program);
}

string ShaderProgram::shaderProgramErrorNames[] = {
	"can't create program",
	"link error",
	"some shader error"
};

}
