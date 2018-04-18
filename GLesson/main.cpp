#include <SFML\Window.hpp>
#include <glbinding\Binding.h>
#include <glbinding\gl\gl.h>
#include <glint\ShaderProgram.h>
#include <iostream>

using namespace gl;
using namespace std;
using namespace vmath;
using namespace opengl;

enum VertexAttribType
{
	POSITION,
	COLOR
};

vec2 positions[] =
{
	vec2(-0.5f, -0.5f),
	vec2(0.f, 0.5f),
	vec2(0.5f, -0.5f)
};

vec3 colors[] =
{
	vec3(1.f, -0.f, 0.f),
	vec3(0.f, 1.f, 0.f),
	vec3(0.f, 0.f, 1.f)
};

int main(int argc, char **argv)
{
    sf::ContextSettings context;
    //context.antialiasingLevel = 8;
    sf::Window window(sf::VideoMode(1280, 720), "GLesson", sf::Style::Default, context);
    window.setVerticalSyncEnabled(true);

	glbinding::Binding::initialize();

    glClearDepth(1.f);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);


	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffers[2];

	glGenBuffers(2, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[POSITION]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * 3, positions, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[COLOR]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 3, colors, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	RenderShaderProgram<SHF_VERTEX | SHF_FRAGMENT> shader(
		Shader<SH_VERTEX>({ "vertex.vert" }),
		Shader<SH_FRAGMENT>({ "fragment.frag" })
		);


	shader.use();
	cout << shader.collectLog() << endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    while (window.isOpen())
    {
        sf::Event event;
        window.pollEvent(event);

		if (event.type == sf::Event::Resized)
			glViewport(0, 0, event.size.width, event.size.height);

		if (event.type == sf::Event::Closed)
			window.close();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
        window.display();
    }

    return 0;
}