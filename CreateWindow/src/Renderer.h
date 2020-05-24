#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak(); // macros
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__,__LINE__)) // 打出具体哪行
void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer {
public:
	void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
	void Clean();
	//void SetClearColor(glm::vec4 clearColor);
};
