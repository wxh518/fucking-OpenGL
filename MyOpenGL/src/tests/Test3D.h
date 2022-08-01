#pragma once

#include "Test.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include <memory>

namespace test {

	class Test3D : public Test
	{
	public:
		Test3D();
		~Test3D();

		static Test3D* g_3dTest;
		void OnUpdate(float deltatime /* = 0.0f */) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void processInput();

	public:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_TextureWall;
		std::unique_ptr<Texture> m_TextureFace;

		glm::mat4 m_Model;
		glm::mat4 m_View;
		glm::mat4 m_Proj;

		glm::vec3 m_CameraPos;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraUp;

		float m_DeltaTime;
		float m_LastFrame;

		float m_LastX;
		float m_LastY;

		float m_Yaw;
		float m_Pitch;
		float m_Fov;

		bool m_IsSetCallBack;
		bool m_IsFirstMouse;
	};

}