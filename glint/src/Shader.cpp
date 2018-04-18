#include <glint/Shader.h>
#include <fstream>


namespace opengl
{

using namespace std;
using namespace gl;

//ShaderObject
ShaderObject::ShaderObject() : shader(0), errorLogFlags(SH_ERR_FAILED_CREATE) {}

ShaderObject::ShaderObject(ShaderObject &&source) :
	shader(source.shader),
	type(source.type),
	errorLogFlags(source.errorLogFlags)
{
	source.errorLogFlags = SH_ERR_FAILED_CREATE;
	source.shader = 0;
}

ShaderObject& ShaderObject::operator=(ShaderObject &&source)
{
	if (this != &source)
	{
		glDeleteShader(shader);
		shader = source.shader;
		type = source.type;
		errorLogFlags = source.errorLogFlags;

		source.errorLogFlags = SH_ERR_FAILED_CREATE;
		source.shader = 0;
	}
	return *this;
}

bool ShaderObject::loadFromFile(const string &path, ostringstream &dst)
{
	ifstream file;
	bool status = false;
	file.open(path, ios::in);

	if (!file.is_open())
		errorLogFlags |= SH_ERR_NO_FILE;
	else
	{
		dst << file.rdbuf() << endl;
		status = true;
	}
	file.close();
	return status;
}

void ShaderObject::create(const string &source)
{
	const GLchar * src = source.c_str();

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLboolean compileStatus = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_FALSE)
		errorLogFlags |= SH_ERR_FAILED_COMPILE;
}

ShaderObject::ShaderObject(ShaderType _type, const string &source, const string &prefix) :
	type(_type),
	errorLogFlags(0),
	shader(glCreateShader(glShaderTypes[unsigned char(_type)]))
{
	if (shader == 0)
	{
		errorLogFlags |= SH_ERR_FAILED_CREATE;
		return;
	}

	create(source);
}

ShaderObject::ShaderObject(ShaderType _type) :
	type(_type),
	errorLogFlags(0),
	shader(glCreateShader(glShaderTypes[unsigned char(_type)]))
{
	if (shader == 0)
	{
		errorLogFlags |= SH_ERR_FAILED_CREATE;
		return;
	}

	create(stdSources[unsigned char(type)]);
}

ShaderObject::ShaderObject(ShaderType _type, const initializer_list<string> &paths, const string &prefix) :
	type(_type),
	errorLogFlags(0),
	shader(glCreateShader(glShaderTypes[unsigned char(_type)]))
{
	if (shader == 0)
	{
		errorLogFlags |= SH_ERR_FAILED_CREATE;
		return;
	}

	ostringstream source(prefix + '\n', ostringstream::ate);

	ifstream file;
	for (auto path : paths)
	{
		loadFromFile(path, source);
	}

	create(source.str());
}

const GLuint ShaderObject::getShader() const
{
	return shader;
}

ShaderType ShaderObject::getType() const
{
	return type;
}

unsigned char ShaderObject::getErrorFlags() const
{
	return errorLogFlags;
}

bool ShaderObject::compileStatus() const
{
	return ((errorLogFlags & SH_ERR_FAILED_COMPILE) == 0);
}

bool ShaderObject::isCreated() const
{
	return ((errorLogFlags & SH_ERR_FAILED_CREATE) == 0);
}

bool ShaderObject::noError() const
{
	return errorLogFlags == 0;
}

string ShaderObject::collectLog() const
{
	if (errorLogFlags == 0)
		return "";
	ostringstream log;
	log << "log for " + shaderTypeExtentions[char(type)] + " shader:" << endl;
	for (unsigned char i = 0; i < SH_ERR_NUMBER; ++i)
		if ((errorLogFlags >> i) & 1)
			log << ' ' << shaderErrorNames[i] << endl;
	if (!compileStatus())
		log << ">>>>>>>>>>compile log<<<<<<<<<<" << endl << getCompileLog() << ">>>>>>>>>>>>>>> <<<<<<<<<<<<<<<" << endl;
	return log.str();
}

string ShaderObject::getCompileLog() const
{
	if (!isCreated())
		return "compile log is not available for uncreated shader\n";

	string tmp;

	GLint logSize = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);

	if (logSize > 0)
	{
		GLchar *log = new GLchar[logSize];
		glGetShaderInfoLog(shader, logSize, nullptr, log);
		tmp.reserve(logSize);
		tmp.assign(log);
		delete[] log;
	}

	return tmp;
}

void ShaderObject::reset()
{
	if (isCreated())
		glDeleteShader(shader);
	shader = 0;
	errorLogFlags = SH_ERR_FAILED_CREATE;
}

ShaderObject::~ShaderObject()
{
	if (isCreated())
		glDeleteShader(shader);
}

const GLenum ShaderObject::glShaderTypes[] = {
	GLenum(GL_VERTEX_SHADER),
	GLenum(GL_FRAGMENT_SHADER),
	GLenum(GL_TESS_CONTROL_SHADER),
	GLenum(GL_TESS_EVALUATION_SHADER),
	GLenum(GL_GEOMETRY_SHADER),
	GLenum(GL_COMPUTE_SHADER)
};

const string ShaderObject::shaderTypeExtentions[] = {
	"vert",
	"frag",
	"tesc",
	"tese",
	"geom",
	"comp"
};

const string ShaderObject::stdSources[] = {
	//VERTEX
	"#version 430 core\n"
	"layout (location = 0) in vec3 position;\n"
	"layout (location = 1) uniform mat4 transform;\n"
	"void main()\n"
	"{\n"
	"gl_Position = transform * vec4(position, 1.0);\n"
	"}\n",

	//FRAGMENT
	"#version 430 core\n"
	"void main()\n"
	"{\n"
	"gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
	"}\n",

	//TODO: TESSELATION_CONTROL
	"void main() {}\n",

	//TODO: TESSELATION_EVALUATION
	"void main() {}\n",

	//TODO: ���������
	"#version 430 core\n"
	"layout(points) in\n;"
	"layout(points, max_vertices = 1) out;\n"
	"void main()\n"
	"{\n"
	"gl_Position = gl_in[0].gl_Position;\n"
	"EmitVertex();\n"
	"EndPrimitive();\n"
	"}\n",

	//TODO: COMPUTE
	"void main() {}\n"
};

const string ShaderObject::shaderErrorNames[] = {
	"can't open file",
	"using standart source",
	"shader is not created",
	"compile error"
};



}
