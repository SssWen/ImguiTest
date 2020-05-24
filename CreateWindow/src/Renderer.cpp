
#include <iostream>
#include "Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR); // = 0
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[Opengl Error] " << error << function <<
			" " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
{
	vao.Bind();
	ibo.Bind();
	shader.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ibo.GetElement(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clean()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}