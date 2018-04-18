#pragma once

#include <glbinding\gl\gl.h>
#include <vmath\Matrix.h>

namespace opengl
{
using namespace gl;
using namespace vmath;

template <class type>
struct Uniform
{
	static type get(GLint location, GLuint program);
	static void set(GLint location, const type &value);
};

/*******************************************Implementation*******************************************/

//float
template <>
struct Uniform<GLfloat>
{
	static void set(GLint location, const GLfloat &value)
	{
		glUniform1f(location, value);
	}

	static GLfloat get(GLint location, GLuint program)
	{
		GLfloat tmp;
		glGetUniformfv(program, location, &tmp);
		return tmp;
	}
};

//float2
template <>
struct Uniform<vec2>
{
	static void set(GLint location, const vec2 &value)
	{
		glUniform2f(location, value[0], value[1]);
	}

	static vec2 get(GLint location, GLuint program)
	{
		GLfloat tmp[2];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//float3
template <>
struct Uniform<vec3>
{
	static void set(GLint location, const vec3 &value)
	{
		glUniform3f(location, value[0], value[1], value[2]);
	}

	static vec3 get(GLint location, GLuint program)
	{
		GLfloat tmp[3];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//float4
template <>
struct Uniform<vec4>
{
	static void set(GLint location, const vec4 &value)
	{
		glUniform4f(location, value[0], value[1], value[2], value[3]);
	}

	static vec4 get(GLint location, GLuint program)
	{
		GLfloat tmp[4];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//float2x2
template <>
struct Uniform<mat2>
{
	static void set(GLint location, const mat2 &value)
	{
		glUniformMatrix2fv(location, 1, GL_FALSE, value.memory);
	}

	static mat2 get(GLint location, GLuint program)
	{
		GLfloat tmp[4];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//float3x3
template <>
struct Uniform<mat3>
{
	static void set(GLint location, const mat3 &value)
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, value.memory);
	}

	static mat3 get(GLint location, GLuint program)
	{
		GLfloat tmp[9];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//float4x4
template <>
struct Uniform<mat4>
{
	static void set(GLint location, const mat4 &value)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, value.memory);
	}

	static mat4 get(GLint location, GLuint program)
	{
		GLfloat tmp[16];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//float2x3
template <>
struct Uniform<mat2x3>
{
	static void set(GLint location, const mat2x3 &value)
	{
		glUniformMatrix2x3fv(location, 1, GL_FALSE, value.memory);
	}

	static mat2x3 get(GLint location, GLuint program)
	{
		GLfloat tmp[6];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//float3x2
template <>
struct Uniform<mat3x2>
{
	static void set(GLint location, const mat3x2 &value)
	{
		glUniformMatrix3x2fv(location, 1, GL_FALSE, value.memory);
	}

	static mat3x2 get(GLint location, GLuint program)
	{
		GLfloat tmp[6];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//float2x4
template <>
struct Uniform<mat2x4>
{
	static void set(GLint location, const mat2x4 &value)
	{
		glUniformMatrix2x4fv(location, 1, GL_FALSE, value.memory);
	}

	static mat2x4 get(GLint location, GLuint program)
	{
		GLfloat tmp[8];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//float4x2
template <>
struct Uniform<mat4x2>
{
	static void set(GLint location, const mat4x2 &value)
	{
		glUniformMatrix4x2fv(location, 1, GL_FALSE, value.memory);
	}

	static mat4x2 get(GLint location, GLuint program)
	{
		GLfloat tmp[8];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//float3x4
template <>
struct Uniform<mat3x4>
{
	static void set(GLint location, const mat3x4 &value)
	{
		glUniformMatrix3x4fv(location, 1, GL_FALSE, value.memory);
	}

	static mat3x4 get(GLint location, GLuint program)
	{
		GLfloat tmp[12];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//float4x3
template <>
struct Uniform<mat4x3>
{
	static void set(GLint location, const mat4x3 &value)
	{
		glUniformMatrix4x3fv(location, 1, GL_FALSE, value.memory);
	}

	static mat4x3 get(GLint location, GLuint program)
	{
		GLfloat tmp[12];
		glGetUniformfv(program, location, tmp);
		return tmp;
	}
};

//integer
template <>
struct Uniform<GLint>
{
	static void set(GLint location, const GLint &value)
	{
		glUniform1i(location, value);
	}


	static GLint get(GLint location, GLuint program)
	{
		GLint tmp;
		glGetUniformiv(program, location, &tmp);
		return tmp;
	}
};

//integer2
template <>
struct Uniform<ivec2>
{
	static void set(GLint location, const ivec2 &value)
	{
		glUniform2i(location, value[0], value[1]);
	}

	static ivec2 get(GLint location, GLuint program)
	{
		GLint tmp[2];
		glGetUniformiv(program, location, tmp);
		return tmp;
	}
};

//integer3
template <>
struct Uniform<ivec3>
{
	static void set(GLint location, const ivec3 &value)
	{
		glUniform3i(location, value[0], value[1], value[2]);
	}

	static ivec3 get(GLint location, GLuint program)
	{
		GLint tmp[3];
		glGetUniformiv(program, location, tmp);
		return tmp;
	}
};

//integer4
template <>
struct Uniform<ivec4>
{
	static void set(GLint location, const ivec4 &value)
	{
		glUniform4i(location, value[0], value[1], value[2], value[3]);
	}

	static ivec4 get(GLint location, GLuint program)
	{
		GLint tmp[4];
		glGetUniformiv(program, location, tmp);
		return tmp;
	}
};

//unsigned integer
template <>
struct Uniform<GLuint>
{
	static void set(GLint location, const GLuint &value)
	{
		glUniform1ui(location, value);
	}

	static GLuint get(GLint location, GLuint program)
	{
		GLuint tmp;
		glGetUniformuiv(program, location, &tmp);
		return tmp;
	}
};

//unsigned integer2
template <>
struct Uniform<uvec2>
{
	static void set(GLint location, const uvec2 &value)
	{
		glUniform2ui(location, value[0], value[1]);
	}

	static uvec2 get(GLint location, GLuint program)
	{
		GLuint tmp[2];
		glGetUniformuiv(program, location, tmp);
		return tmp;
	}
};

//unsigned integer3
template <>
struct Uniform<uvec3>
{
	static void set(GLint location, const uvec3 &value)
	{
		glUniform3ui(location, value[0], value[1], value[2]);
	}

	static uvec3 get(GLint location, GLuint program)
	{
		GLuint tmp[3];
		glGetUniformuiv(program, location, tmp);
		return tmp;
	}
};

//unsigned integer4
template <>
struct Uniform<uvec4>
{
	static void set(GLint location, const uvec4 &value)
	{
		glUniform4ui(location, value[0], value[1], value[2], value[3]);
	}

	static uvec4 get(GLint location, GLuint program)
	{
		GLuint tmp[4];
		glGetUniformuiv(program, location, tmp);
		return tmp;
	}
};

//bool
template<>
struct Uniform<bool>
{
	static void set(GLint location, const bool &value)
	{
		GLboolean tmp = value ? GL_TRUE : GL_FALSE;
		glUniform1i(location, GLint(value));
	}

	static bool get(GLint location, GLuint program)
	{
		GLint tmp;
		glGetUniformiv(program, location, &tmp);
		return (tmp == 0) ? false : true;
	}
};

}
