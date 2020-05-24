
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include<fstream>
#include<string>
#include<sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"




int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);
	if (glewInit() != GLEW_OK)
		std::cout << "Error!" << std::endl;
	std::cout << glGetString(GL_VERSION) << " : "  <<sizeof(unsigned int) << std::endl;
	

	
		float position[] = {
			//-0.5f, -0.5f, 0.0f, 0.0f, // 0
			// 0.5f, -0.5f, 1.0f, 0.0f, // 1
			// 0.5f,  0.5f, 1.0f, 1.0f, // 2
			//-0.5f,  0.5f, 0.0f, 1.0f  // 3
		   -50.0f, -50.0f, 0.0f, 0.0f, // 0
			50.0f, -50.0f, 1.0f, 0.0f, // 1
			50.0f,  50.0f, 1.0f, 1.0f, // 2
		   -50.0f,  50.0f, 0.0f, 1.0f  // 3

		};

		unsigned int indices[] = {
			0,1,2,
			2,3,0
		};
		
		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		VertexArray va;
		// bind buffer
		VertexBuffer vb(position, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2); // 添加坐标attribute属性,layout 的stride也确定了
		layout.Push<float>(2); // 添加uv坐标attribute,
		va.AddBuffer(vb,layout);

		// index buffer
		IndexBuffer ib(2 * 3 * sizeof(unsigned int), indices);

		glm::vec3 translation(200, 200, 0);
		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);		
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));						
		glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 MVP = proj * view * model;

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		//shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/ChernoLogo1.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);
		shader.SetUniformMat4f("u_MVP", MVP);


		va.UnBind();
		vb.UnBind();
		ib.UnBind();
		shader.Unbind();

		float increment = 0.05f;
		float r = 0;

		Renderer renderer;

		// Setup ImGui binding
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


		
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
		
			renderer.Clean();		
			

			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;

			ImGui_ImplGlfwGL3_NewFrame();
			// 1. Show a simple window.
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
			{
				ImGui::SliderFloat3("translationA", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 MVP = proj * view * model;
				shader.SetUniformMat4f("u_MVP", MVP);
				renderer.Draw(va, ib, shader);				
			}
			{
				ImGui::SliderFloat3("translationB", &translationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 MVP = proj * view * model;
				shader.SetUniformMat4f("u_MVP", MVP);
				renderer.Draw(va, ib, shader);
			}
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); 

		// imgui
		/*	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);*/
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		//glDeleteProgram(shader);
	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}