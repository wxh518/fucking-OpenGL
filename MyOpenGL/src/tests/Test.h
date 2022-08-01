#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

class GLFWwindow;

namespace test{

	class Test
	{
	public:
		Test() : m_Window(nullptr) {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltatime = 0.0f) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

		void SetWindow(GLFWwindow* window) { m_Window = window; }
	protected:
		GLFWwindow* m_Window;
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentPointer);
		~TestMenu();

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test: " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};

}