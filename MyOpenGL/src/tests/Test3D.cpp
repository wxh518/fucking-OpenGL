#include "Test3D.h"

#include "Renderer.h"
#include "VertexBufferLayout.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

#include "GLFW/glfw3.h"

namespace test {

	Test3D* Test3D::g_3dTest = nullptr;
	Test3D::Test3D()
		: m_Model(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Proj(glm::mat4(1.0f)),
		  m_DeltaTime(0.0f), m_LastFrame(0.0f), m_LastX(960.0f / 2), m_LastY(540.0f / 2),
		  m_Yaw(-90.0f), m_Pitch(0.0f), m_Fov(45.0f), m_IsSetCallBack(false), m_IsFirstMouse(true)
	{ 
		Test3D::g_3dTest = this;

		m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, -3.0f));
		m_Proj = glm::perspective(glm::radians(45.0f), (float)960 / (float)540, 0.1f, 100.0f);

		m_CameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
		m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

		float positions[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GLCall(glEnable(GL_DEPTH_TEST));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 5 * 36 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_Shader = std::make_unique<Shader>("res/shaders/3D.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		m_Shader->SetUniformMat4f("view", m_View);
		m_Shader->SetUniformMat4f("projection", m_Proj);

		m_TextureWall = std::make_unique<Texture>("res/textures/container.jpg");
		m_TextureFace = std::make_unique<Texture>("res/textures/awesomeface.png");
		m_TextureWall->Bind();
		m_TextureFace->Bind(1);
        m_Shader->SetUniform1i("u_TextureWall", 0);
		m_Shader->SetUniform1i("u_TextureFace", 1);

        m_VAO->UnBind();
        m_VertexBuffer->UnBind();
        m_IBO->UnBind();
        m_Shader->UnBind();
	}

	Test3D::~Test3D()
	{
		glfwSetCursorPosCallback(m_Window, nullptr);
		glfwSetScrollCallback(m_Window, nullptr);
	}

	void Test3D::OnUpdate(float deltatime /* = 0.0f */)
	{

	}

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	void Test3D::OnRender()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_TextureWall->Bind();
		m_TextureFace->Bind(1);
        m_Shader->Bind();
		m_VAO->Bind();

		processInput();

		//float radius = 10.0f;
		//float camX = sin(glfwGetTime()) * radius;
		//float camZ = cos(glfwGetTime()) * radius;
		//glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0),
		//	glm::vec3(camX, 1.0, camZ));
		//m_View = view;
		//m_Shader->SetUniformMat4f("view", m_View);

		m_Shader->SetUniformMat4f("view", m_View);
		m_Shader->SetUniformMat4f("projection", m_Proj);

		for (int i = 1; i <= 10; i++)
		{
			m_Model = glm::mat4(1.0f);
			m_Model = glm::translate(m_Model, cubePositions[i - 1]);
			float angle = 20.0f * i;
			m_Model = glm::rotate(m_Model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_Shader->SetUniformMat4f("model", m_Model);

			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}		

	}

	void Test3D::OnImGuiRender()
	{

	}

	void mouseCallBack(GLFWwindow* window, double xpos, double ypos)
	{
		if (Test3D::g_3dTest->m_IsFirstMouse)
		{
			Test3D::g_3dTest->m_LastX = xpos;
			Test3D::g_3dTest->m_LastY = ypos;
			Test3D::g_3dTest->m_IsFirstMouse = false;
		}

		float xoffset = xpos - Test3D::g_3dTest->m_LastX;
		float yoffset = Test3D::g_3dTest->m_LastY - ypos;
		Test3D::g_3dTest->m_LastX = xpos;
		Test3D::g_3dTest->m_LastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		Test3D::g_3dTest->m_Yaw += xoffset;
		Test3D::g_3dTest->m_Pitch += yoffset;

		if (Test3D::g_3dTest->m_Pitch > 89.0f)
			Test3D::g_3dTest->m_Pitch = 89.0f;
		if (Test3D::g_3dTest->m_Pitch < -89.0f)
			Test3D::g_3dTest->m_Pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(Test3D::g_3dTest->m_Pitch)) * cos(glm::radians(Test3D::g_3dTest->m_Yaw));
		front.y = sin(glm::radians(Test3D::g_3dTest->m_Pitch));
		front.z = cos(glm::radians(Test3D::g_3dTest->m_Pitch)) * sin(glm::radians(Test3D::g_3dTest->m_Yaw));
		Test3D::g_3dTest->m_CameraFront = glm::normalize(front);

		glm::mat4 view = glm::lookAt(Test3D::g_3dTest->m_CameraPos, 
			Test3D::g_3dTest->m_CameraPos + Test3D::g_3dTest->m_CameraFront, 
			Test3D::g_3dTest->m_CameraUp);
		Test3D::g_3dTest->m_View = view;
	}

	void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (Test3D::g_3dTest->m_Fov >= 1.0f && Test3D::g_3dTest->m_Fov <= 45.0f)
			Test3D::g_3dTest->m_Fov -= yoffset;
		if (Test3D::g_3dTest->m_Fov <= 1.0f)
			Test3D::g_3dTest->m_Fov = 1.0f;
		if (Test3D::g_3dTest->m_Fov >= 45.0f)
			Test3D::g_3dTest->m_Fov = 45.0f;

		glm::mat4 projection = glm::perspective(glm::radians(Test3D::g_3dTest->m_Fov),
			960.0f / 540.0f, 0.1f, 100.0f);
		Test3D::g_3dTest->m_Proj = projection;
	}

	void Test3D::processInput()
	{
		if (m_Window != nullptr)
		{
			if (!m_IsSetCallBack)
			{
				m_IsSetCallBack = true;
				glfwSetCursorPosCallback(m_Window, mouseCallBack);
				glfwSetScrollCallback(m_Window, scrollCallBack);
			}

			float currentFrame = glfwGetTime();
			m_DeltaTime = currentFrame - m_LastFrame;
			m_LastFrame = currentFrame;

			float speed = 2.5f * m_DeltaTime;
			if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
			{
				m_CameraPos += speed * m_CameraFront;
			}
			else if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
			{
				m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * speed;
			}
			else if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
			{
				m_CameraPos -= speed * m_CameraFront;
			}
			else if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
			{
				m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * speed;
			}
			else if (glfwGetKey(m_Window, GLFW_KEY_P) == GLFW_PRESS)
			{
				glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
			else if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
			{
				glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				glfwSetInputMode(m_Window, GLFW_MOUSE_BUTTON_LEFT, 0);
			}

			glm::mat4 view = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
			m_View = view;
		}
	}

}